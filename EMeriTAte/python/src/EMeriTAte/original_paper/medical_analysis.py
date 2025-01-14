import datetime
import json
import sys

import ciso8601
import pandas
from loguru import logger

from EMeriTAte.original_paper.parsing_medical_data import compute, getFolder, asWeekLog, exploseTimeVariations, \
    correlate_event_with_numerical_vars
from EMeriTAte.timeseries.BuonaGiornata import BuonaGiornata
from EMeriTAte.timeseries.DrugHalfLifeSimulator import Chemists
from EMeriTAte.timeseries.Log import Log
from EMeriTAte.timeseries.MultiTraceIndexing import MultiTraceIndexing
# from EMeriTAte.timeseries.SequentialPatternMining import SequentialPatternMining, MiningConfiguration
from EMeriTAte.timeseries.DTMining import mine_binary_growth_patterns
# from prefixspan import PrefixSpan

def extendDictionaryWithTime(d):
    t = d.log.projectProperties("time", lambda l: min(filter(lambda x : isinstance(x, str), l)))
    d.dayStart = ciso8601.parse_datetime(t[0]).date()
    d.startTime = ciso8601.parse_datetime(t[0]).time()
    d.dayEnd = ciso8601.parse_datetime(t[-1]).date()
    d.endTime = ciso8601.parse_datetime(t[-1]).time()
    return d

def pair_analysis(x, y):
    pass

def performMiningOverAnalysedLog(analysis_log, toPertainYesEvents, timefield):
        L = BuonaGiornata(lambda x: x.startswith("day"))
        skipOsmeh = len(toPertainYesEvents) == 0
        Osmeh = None if skipOsmeh else dict()
        for idx2, trace in enumerate(analysis_log.log):
            if not skipOsmeh:
                WW_count = 0
                WW_curr = None
                for xyz in toPertainYesEvents:
                    if trace.trace_name.endswith(xyz):
                        WW_curr = xyz
                        WW_count += 1
                        if WW_count >= 2:
                            break
                if WW_count == 1:
                    YY = [abc for abc in toPertainYesEvents if abc[1].activityLabel[0]=="Y"]
                    if len(YY)>0:
                        Osmeh[WW_curr] = YY
            pos = None
            neg = None
            actione = None
            doSkip = False
            for activityLabel in trace.positional_events.keys():
                if activityLabel == "Start":
                    continue
                elif (pos is None) and (neg is None):
                    if (activityLabel.startswith("N")):
                        neg = activityLabel
                        actione = activityLabel[1:]
                    elif (activityLabel.startswith("Y")):
                        pos = activityLabel
                        actione = activityLabel[1:]
                    else:
                        doSkip = True
                        break
                elif activityLabel.startswith("N"):
                    neg = activityLabel
                    break
                elif activityLabel.startswith("Y"):
                    pos = activityLabel
                    break
            if doSkip:
                continue
            else:
                if (pos is None) and (neg is None):
                    raise Exception("Unexpected situation!")
                elif pos is None:
                    pos = "Y"+neg[1:]
                elif neg is None:
                    neg = "N"+pos[1:]
                mine_binary_growth_patterns(trace, trace.length, pos, neg, actione, L)
        finalised = L.finalise(timefield)
        if (not skipOsmeh) and (len(finalised)>0):
            for itemo in Osmeh.values():
                for elemento in itemo:
                    elemento[1].activityLabel = elemento[1].activityLabel[1:]
                    finalised[elemento[0]].append(elemento[1])
        return finalised

# doLinearised = False
# global_conf = MiningConfiguration.polyadic_spade(0.8, maxsize=2, maxlen=2)
c = Chemists("/home/giacomo/projects/sdd-processing/sdd-processing/i_want_a_new_drug.json")
class MedicalAnalysis:
    def __init__(self, conf):
        logger.remove(0)
        f = sys.stdout
        if conf.logging_file is not None:
            f = conf.logging_file
        logger.add(f, level=conf.logging_level)
        self.collectDrugsName = set()
        self.environments = {k: compute(k, getFolder(conf.folder, k), self.collectDrugsName, c) for k in conf.patients}
        self.conf = conf
        # global_conf.doMining = self.conf.doMining

    def dump_raw_data_as_csv(self):
        for key, df in self.environments.items():
            df.to_csv(key+".csv", index=False)

    def _patient_specific_time_continous_analysis(self, x):
        # logger.info("Performining the continuous analysis for "+x)
        # logger.trace("1. Data Pre-Processing")
        EntireWeekLog = asWeekLog([(self.environments[x], x)])
        originalChunks = dict()
        tmp = EntireWeekLog
        assert len(tmp.traces)==1
        booleans, floats = exploseTimeVariations(EntireWeekLog, self.conf.epsilon, self.conf.maxval, {'span', 'day'}.union(self.collectDrugsName), "time")
        for drug in self.collectDrugsName:
            floats[drug] = [drug, "time"]
        EntireWeekLog = EntireWeekLog.EventExtraction(booleans, floats, True)
        for idx, row in enumerate(tmp.traces[0].events):
            row.setValue("__class", row.activityLabel)
            row.activityLabel = "__raw_data"
            originalChunks[datetime.datetime.fromisoformat(row["fulltime"])] = [row]

        # logger.trace("2. Weekly data separation into immediate previous and immediate afterwards+mining")
        ewl_idx = MultiTraceIndexing(EntireWeekLog)
        time_continuous_analysis = ewl_idx.segmentByXTraceEventLabel(x + '@label')
        count = 1
        for idx, analysis in enumerate(time_continuous_analysis):
            if analysis.label == "Off":
                if idx == 0:
                    continue
                else:
                    logger.trace(" - Now analysis #"+str(count))
                    polyL = performMiningOverAnalysedLog(analysis.log, self.collectDrugsName)
                    for i in range(len(polyL)):
                        if len(polyL[i]) > 0:
                            t = min(
                                map(lambda x: datetime.datetime.fromisoformat(str(x.getValue("time"))), polyL[i]))
                            assert all(map(lambda x: datetime.datetime.fromisoformat(str(x.getValue("time"))) == t,
                                           polyL[i]))
                            assert t in originalChunks
                            originalChunks[t][0].setValue("__label", analysis.label)
                            originalChunks[t] = originalChunks[t] + polyL[i]
                    logger.trace(" - Past analysis #"+str(count))
                    polyR = performMiningOverAnalysedLog(time_continuous_analysis[idx - 1].log, self.collectDrugsName)
                    for i in range(len(polyR)):
                        if len(polyR[i]) > 0:
                            t = min(
                                map(lambda x: datetime.datetime.fromisoformat(str(x.getValue("time"))), polyR[i]))
                            assert all(map(lambda x: datetime.datetime.fromisoformat(str(x.getValue("time"))) == t,
                                           polyR[i]))
                            assert t in originalChunks
                            originalChunks[t][0].setValue("__label", time_continuous_analysis[idx - 1].label)
                            originalChunks[t] = originalChunks[t] + polyR[i]
                    count = count+1
            if analysis.label == "Off" and idx == 0:
                continue
        return [originalChunks[t] for t in sorted(originalChunks.keys())]


    def singlePatientWeekly(self):
        if self.conf.doTimeContinuous and (self.conf.singlePatient is not None):
            return self._patient_specific_time_continous_analysis(self.conf.singlePatient)


    def logForAllPatientsAfterCollection(self):
        UserLog = Log()
        for pat in self.conf.patients:
            ls = self._patient_specific_time_continous_analysis(pat)
            UserLog.addTracePositional(ls, withData=True, isTab=True,
                                                      withExplicitPayloadMap={"user":pat})
        UserLog.indexing()
        with open("../here/log_weekly.json", "w") as outfile:
            json.dump(UserLog.toJSONObject(), outfile, indent=4)


    def all_patients_analysis(self):
        if self.conf.allPatients:
            timed = pandas.concat(self.environments.values(), axis=0, ignore_index=True)
            timed.to_csv(("+".join(self.conf.patients))+".csv", index=False)
            correlate_event_with_numerical_vars(timed, "+".join(self.conf.patients))
            self.logForAllPatientsAfterCollection()
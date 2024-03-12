import datetime
import itertools
import json
import sys
from collections import OrderedDict, defaultdict
from math import isnan

import ciso8601
import pandas
from intervaltree import IntervalTree
from loguru import logger
from sklearn.metrics import accuracy_score
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier

import timeseries.Utils
import utils
from parsing_medical_data import compute, getFolder, asWeekLog, exploseTimeVariations, \
    correlate_event_with_numerical_vars, asDailyLog
from timeseries.BuonaGiornata import BuonaGiornata
from timeseries.DrugHalfLifeSimulator import Chemists
from timeseries.Log import Log, Event, EventPayload
from timeseries.MultiTraceIndexing import MultiTraceIndexing
from timeseries.SequentialPatternMining import SequentialPatternMining, MiningConfiguration
from timeseries.TimeSeriesMining import mine_binary_growth_patterns
from prefixspan import PrefixSpan

def extendDictionaryWithTime(d):
    t = d.log.projectProperties("time", lambda l: min(filter(lambda x : isinstance(x, str), l)))
    d.dayStart = ciso8601.parse_datetime(t[0]).date()
    d.startTime = ciso8601.parse_datetime(t[0]).time()
    d.dayEnd = ciso8601.parse_datetime(t[-1]).date()
    d.endTime = ciso8601.parse_datetime(t[-1]).time()
    return d

class DailyLogAnalysis:
    def __init__(self, x, t, conf):
        self.user = x
        self.t = t
        self.conf = conf
        logger.trace("1. Preliminary Daily data loading")
        self.DailyLog = asDailyLog(t)
        logger.trace("2. Explosing Time Variations")
        booleans, keys = exploseTimeVariations(self.DailyLog, conf.epsilon, conf.maxval, None, "time")
        self.DailyLog = self.DailyLog.EventExtraction(booleans, keys, False)
        self.DailyLog = MultiTraceIndexing(self.DailyLog)
        logger.trace("3. Effectively grouping traces by day + all_days")
        self.dl_idx = self.DailyLog.groupTraceByLabelCondition(lambda x: x.split("@")[0].split("_")[1],
                                                     lambda x: (x.split("@")[0].split("_")[0]) + "@" + x.split("@")[1])
        self.all_days = set(self.dl_idx.keys())
        self.isIntervalTreeDone = False
        self._interval_tree()
        self.areIntervalsCollected = False
        self.conflicting_intervals = list() # The intervals being off or ok in different days. This will remark to better assess these situations first!
        self._collect_intervals()

    def _interval_tree(self):
        if not self.isIntervalTreeDone:
            logger.trace("4. Distinct Intervals")
            self.tree = IntervalTree()
            for key in self.dl_idx:
                D = self.dl_idx[key].segmentByXTraceEventLabel(self.user+'@label', extendDictionaryWithTime)
                for i in range(len(D)):
                    if i == 0:
                        continue
                    D[i - 1].endTime = D[i].startTime  # (datetime.datetime.combine(datetime.datetime.today(), D[i]["startTime"])-datetime.timedelta(microseconds=1)).time()
                for d in D:
                    self.tree.addi(d.startTime, d.endTime)
            self.tree.split_overlaps()
            self.i = self.tree.all_intervals
            self.isIntervalTreeDone = True

    def _collect_intervals(self):
        self._interval_tree()
        if not self.areIntervalsCollected:
            logger.info("5. Analysing intervals across days")
            self.collected_intervals = OrderedDict()  ## Time Interval -> Day -> (ClassLabel@String, MultiTraceIndexing)
            for key in self.dl_idx:
                L = list(filter(lambda y: not y[1].hasLogWithEmptyTraces(),
                                map(lambda interval: tuple([interval, self.dl_idx[key].subtraceFromInterval("time", lambda
                                    x: ciso8601.parse_datetime(x).time(), interval)]), self.i))
                         )
                if (len(L) > 0):
                    for interval, chunk in L:
                        t = chunk.getTraceByName(self.user + '@label')
                        # assert len(t.positional_events) == 1
                        for label in t.positional_events:
                            if interval.begin not in self.collected_intervals:
                                self.collected_intervals[interval.begin] = dict()
                            self.collected_intervals[interval.begin][key] = (label, chunk)
            self.collected_intervals = OrderedDict(sorted(self.collected_intervals.items()))
            for k, v in self.collected_intervals.items():
                labels = set()
                for day, (label, chunk) in v.items():
                    labels.add(label)
                if (len(labels) > 1):
                    self.conflicting_intervals.append(k)
            self.all_intervals = sorted(list(self.collected_intervals.keys()))
            for interval in self.all_intervals:
                U_interval = self.collected_intervals[interval]
                W_dict = dict()
                for day in self.all_days:
                    if day not in U_interval:
                        W_dict[day] = ('missing', None)
                    else:
                        W_dict[day] = U_interval[day]
                self.collected_intervals[interval] = W_dict
            self.areIntervalsCollected = True


def pair_analysis(x, y):
    pass


def performMiningOverAnalysedLog(analysis_log, conf, toPertainYesEvents, toDiscardEvents, D):
        L = BuonaGiornata(lambda x: x.startswith("day"))
        Osmeh = dict()
        for idx2, trace in enumerate(analysis_log.log):
            WW = list(filter(lambda xyz: trace.trace_name.endswith(xyz), toPertainYesEvents))
            if len(WW) == 1:
                YY = list(filter(lambda abc: abc[1].activityLabel[0]=="Y", enumerate(trace.events)))
                if len(YY)>0:
                    Osmeh[WW[0]] = YY
            pos = None
            neg = None
            actione = None
            doSkip = False
            for key in trace.positional_events.keys():
                if key == "Start":
                    continue
                elif (pos is None) and (neg is None):
                    if (key.startswith("N")):
                        neg = key
                        actione = key[1:]
                    elif (key.startswith("Y")):
                        pos = key
                        actione = key[1:]
                    else:
                        doSkip = True
                        break
                elif key.startswith("N"):
                    # assert pos is not None
                    # assert pos[1:] == key[1:]
                    neg = key
                    break
                elif key.startswith("Y"):
                    # assert neg is not None
                    # assert neg[1:] == key[1:]
                    pos = key
                    break
            if doSkip:
                continue
            else:
                # assert trace.trace_name is not None
                #analysis.outcome_mining[trace.trace_name] =
                mine_binary_growth_patterns(trace, trace.length, pos, neg, actione, conf, L)
        finalised = L.finalise()
        if (len(finalised)>0):
            for itemo in Osmeh.values():
                for elemento in itemo:
                    elemento[1].activityLabel = elemento[1].activityLabel[1:]
                    finalised[elemento[0]].append(elemento[1])
        return finalised

# doLinearised = False
global_conf = MiningConfiguration.polyadic_spade(0.8, maxsize=2, maxlen=2)
c = Chemists("i_want_a_new_drug.json")
class MedicalAnalysis:
    def __init__(self, conf):
        logger.remove(0)
        f = sys.stdout
        if conf.logging_file is not None:
            f = conf.logging_file
        logger.add(f, level=conf.logging_level)
        self.collectDrugsName = set()
        self.patients_raw_data = {k: compute(k, getFolder(conf.folder, k), self.collectDrugsName, c) for k in conf.patients}
        self.conf = conf
        global_conf.doMining = self.conf.doMining

    def _patient_specific_time_continous_analysis(self, x):
        logger.info("Performining the continuous analysis for "+x)
        logger.trace("1. Data Pre-Processing")
        EntireWeekLog = asWeekLog([(self.patients_raw_data[x], x)])
        originalChunks = dict()
        # for _, row in self.patients_raw_data[x].iterrows():
        #     toDict =  dict(row)
        #     time = toDict["fulltime"]
        #     del toDict["fulltime"]
        #     originalChunks[time] = {k: toDict[k] for k in toDict if (not isinstance(toDict[k], float)) or (not isnan(toDict[k])) }
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
        # logger.trace("2. Dumping Pre-processed data")
        #     outfile.write(EntireWeekLog.toHRF())

        logger.trace("2. Weekly data separation into immediate previous and immediate afterwards+mining")
        ewl_idx = MultiTraceIndexing(EntireWeekLog)
        time_continuous_analysis = ewl_idx.segmentByXTraceEventLabel(x + '@label')
        # comparison = list()
        count = 1
        # classOff = {'__class': 'Off'}
        # classOk = {'__class': 'Ok'}
        # mainStream = []
        # TimeSeriesLogOff = Log()
        # TimeSeriesLogOk = Log()
        for idx, analysis in enumerate(time_continuous_analysis):
            if analysis.label == "Off":
                if idx == 0:
                    continue
                else:
                    logger.trace(" - Now analysis #"+str(count))
                    polyL = performMiningOverAnalysedLog(analysis.log, global_conf, self.collectDrugsName, None, None)
                    for i in range(len(polyL)):
                        if len(polyL[i]) > 0:
                            t = min(
                                map(lambda x: datetime.datetime.fromisoformat(str(x.getValue("time"))), polyL[i]))
                            assert all(map(lambda x: datetime.datetime.fromisoformat(str(x.getValue("time"))) == t,
                                           polyL[i]))
                            assert t in originalChunks
                            originalChunks[t][0].setValue("__label", analysis.label)
                            originalChunks[t] = originalChunks[t] + polyL[i]
                            # finalised[idx].insert(0, Event("__raw_data", EventPayload(D[t].items())))
                    # analysis.log.log.addTracePositional(polyL, withData=True, isTab=True)
                    # TotalLog.addTracePositional(polyL, withData=True, isTab=True, withExplicitPayloadMap={"label":analysis.label, "series":idx-1})
                    # TimeSeriesLogOff.append(polyL)
                    logger.trace(" - Past analysis #"+str(count))
                    polyR = performMiningOverAnalysedLog(time_continuous_analysis[idx - 1].log, global_conf, self.collectDrugsName, None, None)
                    # TimeSeriesLogOk.append(polyR)
                    for i in range(len(polyR)):
                        if len(polyR[i]) > 0:
                            t = min(
                                map(lambda x: datetime.datetime.fromisoformat(str(x.getValue("time"))), polyR[i]))
                            assert all(map(lambda x: datetime.datetime.fromisoformat(str(x.getValue("time"))) == t,
                                           polyR[i]))
                            assert t in originalChunks
                            originalChunks[t][0].setValue("__label", time_continuous_analysis[idx - 1].label)
                            originalChunks[t] = originalChunks[t] + polyR[i]
                    # TotalLog.addTracePositional(polyR, withData=True, isTab=True,
                    #                                  withExplicitPayloadMap={"label": time_continuous_analysis[idx - 1].label,
                    #                                                          "series": idx - 1})
                    # time_continuous_analysis[idx - 1].log.log.addTracePositional(polyR, withData=True, isTab=True)
                    # pair_analysis(analysis.outcome_mining, time_continuous_analysis[idx - 1].outcome_mining)
                    # comparison.append(tuple([time_continuous_analysis[idx - 1], analysis]))
                    count = count+1
                    # mainStream.append(polyL)
                    # mainStream.append(polyR)
            if analysis.label == "Off" and idx == 0:
                continue


        return [originalChunks[t] for t in sorted(originalChunks.keys())]

        ## It appears that none of the events happening at the same time are strongly correlated between each other,
        ## as they do not necessarily cover the same span
        # logger.trace("3. Determining strong correlation within events to be removed")
        # sL = set()
        # for x in TimeSeriesLogOk:
        #     for y in x:
        #         for z in y:
        #             sL.add(z.activityLabel)
        # candidates = list(itertools.combinations(sL,2))
        # removeCandidates = set()
        # for x in TimeSeriesLogOff:
        #     for y in x:
        #         dd = defaultdict(lambda: 0)
        #         for z in y:
        #             dd[z.activityLabel] = int(z.getValue("span"))
        #         for idx, (a,b) in enumerate(candidates):
        #             if (dd[a] != dd[b]):
        #                 removeCandidates.add(idx)
        #         candidates = [i for j, i in enumerate(candidates) if j not in removeCandidates]
        #         if len(candidates) == 0:
        #             break
        #     if len(candidates) == 0:
        #         break

        # TimeSeriesLogOff.indexing()
        # TimeSeriesLogOk.indexing()

        ## This analisys just correlates each OK period with an Off period, and does make no analisys between
        ## the same time inverval throughout the week
        # with open("log_weekly_tsa_off_"+x+".json", "w") as outfile:
        #     json.dump(TimeSeriesLogOff.toJSONObject(classOff),outfile,indent=4)
        # with open("log_weekly_tsa_ok_"+x+".json", "w") as outfile:
        #     json.dump(TimeSeriesLogOk.toJSONObject(classOk),outfile,indent=4)
        # logger.trace("4. Preforming Straightforward learning")
        # candidates = dict()
        # for L, R in comparison:
        #     assert L.label == "Ok"
        #     assert R.label == "Off"
        #     candidates = SequentialPatternMining.collect_candidates(candidates, L, R, global_conf)
        #
        # dict_list = []
        # isPolyadic = True
        # isPolyadicFull = True
        # for idx, cp in enumerate(comparison):
        #     L, R = cp
        #     assert L.label == "Ok"
        #     assert R.label == "Off"
        #     test, test_full = SequentialPatternMining.probe_candidates(candidates, dict_list, L, R, global_conf)
        #     isPolyadic = isPolyadic and test
        #     isPolyadicFull = isPolyadicFull and test_full
        #
        # if isPolyadic:
        #     POS = []
        #     NEG = []
        #     for pos, neg in timeseries.Utils.grouped(dict_list, 2):
        #         POS.append(pos)
        #         NEG.append(neg)
        #     SequentialPatternMining.probe_candidates_polyadic(candidates, POS, NEG, global_conf)
        #
        #
        #
        # dict_list = pandas.DataFrame.from_dict(dict_list, orient='columns')
        # dict_list.fillna(0, inplace=True)
        # # timed['label'] = timed['label'].map({'Amantadine 100mg':1, 'Madopar 50/12.5mg':2,'Madopar CR 100/25mg':3, 'Rotigotine':4,'Stalevo 125/31.5/200mg':5,'Sample':0})
        # X = dict_list.drop(labels=['class'], axis=1)
        # y = dict_list['class']
        # X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, stratify=y)
        #
        # rf = DecisionTreeClassifier(max_depth=10)
        # rf.fit(X_train, y_train)
        # y_pred = rf.predict(X_test)
        # accuracy = accuracy_score(y_test, y_pred)
        # print("Accuracy:", accuracy)
        # print(candidates)
        # return (ewl_idx, comparison)
        ## TODO: mining within all the Ok regions,
        ##       mining within all the Off regions
        ##       learning the patterns distinguishing the two
        ##       keeping the relevant pattern within each region
        ##       establishing correlation between each pair within comparison



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
        with open("log_weekly.json", "w") as outfile:
            json.dump(UserLog.toJSONObject(), outfile, indent=4)


    def all_patients_analysis(self):
        if self.conf.allPatients:
            timed = pandas.concat(self.patients_raw_data.values(), axis=0, ignore_index=True)
            timed.to_csv(("+".join(self.conf.patients))+".csv", index=False)
            correlate_event_with_numerical_vars(timed, "+".join(self.conf.patients))
            self.logForAllPatientsAfterCollection()

    def patient_specific_daily_analysis(self, x):
        logger.info("Performining the daily analysis for "+x)
        d = DailyLogAnalysis(x, [(self.patients_raw_data[x], x)], self.conf)
        originalChunks = dict()
        for _, row in self.patients_raw_data[x].iterrows():
            toDict =  dict(row)
            time = toDict["fulltime"]
            del toDict["fulltime"]
            originalChunks[time] = {k: toDict[k] for k in toDict if (not isinstance(toDict[k], float)) or (not isnan(toDict[k])) }
        TimeSeriesLogs = {'Off': Log(), 'Ok': Log()}
        classes = {'Off':{'__class': 'Off'},'Ok':{'__class': 'Ok'}}
        # TimeSeriesLogOff = Log()
        # TimeSeriesLogOk = Log()
        for startIntervalTime, dictForDays in d.collected_intervals.items():
            for day, (clazz,MTI) in dictForDays.items():
                if MTI is not None:
                    logger.trace(" - analysis day=" + str(day) +" at="+str(startIntervalTime)+" clazz="+str(clazz))
                    dictLocal = {}
                    dictLocal["startIntervalTime"] = str(startIntervalTime)
                    dictLocal["__class"] = clazz
                    dictLocal["day"] = day
                    traces = performMiningOverAnalysedLog(MTI, global_conf, self.collectDrugsName, None, originalChunks)
                    TimeSeriesLogs[clazz].addTracePositional(traces, withData=True, isTab=True,
                                                     withExplicitPayloadMap=dictLocal)
        for key, log in TimeSeriesLogs.items():
            with open("log_daily_w_interval_tsa_"+key.lower()+"_" + x + ".json", "w") as outfile:
                json.dump(log.toJSONObject(classes[key]), outfile, indent=4)

    def singlePatientDaily(self):
        if self.conf.doDailyBasis and (self.conf.singlePatient is not None):
            return self.patient_specific_daily_analysis(self.conf.singlePatient)

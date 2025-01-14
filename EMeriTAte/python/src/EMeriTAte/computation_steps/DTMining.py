import datetime
import glob
import json
import os
from pathlib import Path

import pandas
from loguru import logger
from EMeriTAte.timeseries.Log import Log, CollectTypeEvidence, TracePositional

from EMeriTAte.original_paper.medical_analysis import performMiningOverAnalysedLog
from EMeriTAte.original_paper.parsing_medical_data import asFinalLog, exploseTimeVariations
from EMeriTAte.timeseries.MultiTraceIndexing import MultiTraceIndexing, OutcomeAnalysis


class DTMining:
    def __init__(self, folder, class_field, time_field, epsilon=0.01, maxval=10000000000000.0, ignore=None, replace=None, conversion=None, toExtendWithTime=None):
        """
        :param folder:      Folder containing the time series parameters splitted by environment file as csv
        :param class_field: The field containing the class information
        :param time_field:  The field after replcament specifying where to retrieve the temporal information, for injecting this information across mined DTs
        :param epsilon:     Sensitivity parameter
        :param maxval:      Maximum value?
        :param ignore:      Fields from the csv file to ignore while performing the DT mining

        :param replace:     Some field replacement per row
        :param conversion:  Converting the numerical class into a textual label
        :param toExtendWithTime: Field determining which element should be associated with time
        """
        if toExtendWithTime is None:
            toExtendWithTime = set()
        self.toExtendWithTime = set(toExtendWithTime)
        self.time_field = time_field
        self.conversion = conversion
        self.replace = replace
        self.class_field = class_field
        if ignore is None:
            ignore = set()
        self.ignore = set(ignore)
        self.maxval = maxval
        self.epsilon = epsilon
        self.environments = dict()
        self.folder = folder
        self.inttype = True

        ## Total loading time
        start = datetime.datetime.now()
        for file in glob.glob(os.path.join(folder, "*.csv")):
            df = pandas.read_csv(file, parse_dates=True)
            try:
                df[time_field] = pandas.to_numeric(df[time_field], downcast="integer").astype(int)
            except:
                self.inttype = False
                df[time_field] = pandas.to_datetime(df[time_field])
            if self.conversion is not None:
                df[class_field] = pandas.to_numeric(df[class_field], downcast="integer").astype(int)
            self.environments[Path(file).stem] = df
            # print(file)
        fini = datetime.datetime.now()
        mining_and_json_ser = fini - start

        self.loading_time = mining_and_json_ser.total_seconds() * 1000.0
        self.serial_time = 0.0
        self.mine_time = 0.0
        self.event_serial_time = 0.0

    def transform(self, filename=None): #, cached=True, concurrent=False):
        ## time reset
        self.serial_time = 0.0
        self.mine_time = 0.0
        self.event_serial_time = 0.0

        if filename is None:
            filename = "log_weekly.json"
        M = {"user":None}
        p = os.path.join(self.folder, filename)
        if True: #(not cached) or (not os.path.isfile(p)): ~~ Always performing the algorithm, no matter what. It would be the higher level to impede this, but mainly for debugging purposes
            start_serial = datetime.datetime.now()
            fp = open(p, "w")
            fp.write('{"log":[')
            end_serial = datetime.datetime.now()
            self.event_serial_time += ((end_serial - start_serial).total_seconds() * 1000.0)
            # UserLog = Log(careAboutUniqueEvents=False)

            start_mining = datetime.datetime.now()
            cle = CollectTypeEvidence()
            n = len(self.environments)
            for idx, pat in enumerate(self.environments):
                M["user"] = pat
                ls = self._perEnvironment(pat, self.time_field)
                # obj, tp = UserLog.addTracePositional(ls, withData=True, isTab=True,
                #                                           withExplicitPayloadMap={"user":pat},
                #                                  explicitlyStoreTrace=False)
                tp = TracePositional(ls, withData=True, withExplicitPayloadMap=M)

                start_serial = datetime.datetime.now()
                obj = tp.toJSONObject()
                obj["__name"] = str(pat)
                cle.collectEvidence(tp)
                fp.write(json.dumps(obj))
                if not (idx == (n - 1)):
                    fp.write("," + os.linesep)
                fp.flush()
                end_serial = datetime.datetime.now()
                self.event_serial_time += ((end_serial - start_serial).total_seconds() * 1000.0)

                del obj
                del tp
                # print(f"{(idx/len(self.environments))*100.0}")


            # if not concurrent:
            #
            # else:
            #     import concurrent.futures
            #     futures = []
            #
            #     with concurrent.futures.ThreadPoolExecutor() as e:
            #         futures2 = {e.submit(self._perEnvironment, pat, self.time_field): pat for pat in self.environments}
            #         idx = 0
            #         for f in concurrent.futures.as_completed(futures2):
            #             pat = futures2[f]
            #             ls = f.result()
            #             tp = TracePositional(ls, withData=True, withExplicitPayloadMap=M)
            #             obj = tp.toJSONObject()
            #             obj["__name"] = str(pat)
            #             cle.collectEvidence(tp)
            #             fp.write(json.dumps(obj))
            #             if not (idx == (n - 1)):
            #                 fp.write("," + os.linesep)
            #             fp.flush()
            #             del obj
            #             del tp
            #             idx += 1



            cle.finalise()
            end_mining = datetime.datetime.now()
            self.mine_time += (((end_mining - start_mining) * 1000.0).total_seconds() - self.event_serial_time)

            start_serial = datetime.datetime.now()
            fp.write('],'+os.linesep+'"schema":')
            fp.write(json.dumps(cle.keyType))
            fp.flush()
            fp.write(',"event_hierarchy":')
            fp.write(json.dumps(cle.deriveHierarchy))
            fp.write('}')
            fp.close()
            end_serial = datetime.datetime.now()
            self.event_serial_time += ((end_serial - start_serial).total_seconds() * 1000)

            start_serial = datetime.datetime.now()
            with open(p, "r") as infile:
                o = json.load(infile)
            #    json.dump(UserLog.toJSONObject(), outfile, indent=4)
            tmp = {"schema": o["schema"], "event_hierarchy": o["event_hierarchy"], "log": o["log"]}
            # UserLog.indexing()
            with open(p, "w") as outfile:
               json.dump(tmp, outfile, indent=4)
            end_serial = datetime.datetime.now()
            self.serial_time = ((end_serial - start_serial).total_seconds() * 1000)
        return p

    def _perEnvironment(self, envName, timedim, doesLabelChangeInTime=False):
        x = envName
        logger.info("Performining the continuous analysis for "+x)
        # logger.trace("1. Data Pre-Processing")
        EntireTimeLog = asFinalLog(self.environments[x], x, self.class_field, self.replace, self.conversion)
        originalChunks = dict()
        tmp = EntireTimeLog
        assert len(tmp.traces)==1
        booleans, floats = exploseTimeVariations(EntireTimeLog, self.epsilon, self.maxval, self.ignore, self.time_field)
        for drug in self.toExtendWithTime:
            floats[drug] = [drug, self.time_field]
        EntireTimeLog = EntireTimeLog.EventExtraction(booleans, floats, True)
        for idx, row in enumerate(tmp.traces[0].events):
            row.setValue("__class", row.activityLabel)
            row.activityLabel = "__raw_data"
            ttt = row[self.time_field]
            try:
                originalChunks[int(ttt)] = [row]
            except:
                originalChunks[datetime.datetime.fromisoformat(ttt)] = [row]
        # logger.trace("2. Weekly data separation into immediate previous and immediate afterwards+mining")
        ewl_idx = MultiTraceIndexing(EntireTimeLog)
        if not doesLabelChangeInTime:
            classes = {x.activityLabel for x in ewl_idx.log.traces[0]}
            if len(classes)>1:
                print("ERROR: Multiple classes. Falling back to the doesLabelChangeInTime=True case")
                self.labelDoesChangeWithTime(ewl_idx, originalChunks, timedim, x)
            else:
                clazz = next(iter({x.activityLabel for x in ewl_idx.log.traces[0]}))
                analysis = OutcomeAnalysis(clazz, 0, ewl_idx, ewl_idx)
                polyL = performMiningOverAnalysedLog(analysis.log, self.toExtendWithTime, timedim)
                self.maximalContigualCollection(analysis, originalChunks, polyL)
        else:
            self.labelDoesChangeWithTime(ewl_idx, originalChunks, timedim, x)
        return [originalChunks[t] for t in sorted(originalChunks.keys())]

    def labelDoesChangeWithTime(self, ewl_idx, originalChunks, timedim, x):
        time_continuous_analysis = ewl_idx.segmentByXTraceEventLabel(x + '@label')
        for idx, analysis in enumerate(time_continuous_analysis):
            polyL = performMiningOverAnalysedLog(analysis.log, self.toExtendWithTime, timedim)
            self.maximalContigualCollection(analysis, originalChunks, polyL)

    def maximalContigualCollection(self, analysis, originalChunks, polyL):
        for i in range(len(polyL)):
            if len(polyL[i]) > 0:
                try:
                    t = min(
                        map(lambda x: int(x.getValue(self.time_field)), polyL[i]))
                    assert all(map(lambda x: int(x.getValue(self.time_field)) == t, polyL[i]))
                except:
                    t = min(map(lambda x: datetime.datetime.fromisoformat(str(x.getValue(self.time_field))), polyL[i]))
                    assert all(map(lambda x: datetime.datetime.fromisoformat(str(x.getValue(self.time_field))) == t,
                                   polyL[i]))
                assert t in originalChunks
                originalChunks[t][0].setValue("__label", analysis.label)
                originalChunks[t] = originalChunks[t] + polyL[i]

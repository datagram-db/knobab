import datetime
import glob
import json
import os
from pathlib import Path

import pandas
from loguru import logger
from EMeriTAte.timeseries.Log import Log

from EMeriTAte.original_paper.medical_analysis import performMiningOverAnalysedLog
from EMeriTAte.original_paper.parsing_medical_data import asFinalLog, exploseTimeVariations
from EMeriTAte.timeseries.MultiTraceIndexing import MultiTraceIndexing


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
        for file in glob.glob(os.path.join(folder, "*.csv")):
            df = pandas.read_csv(file, parse_dates=True)
            df[time_field] = pandas.to_datetime(df[time_field])
            self.environments[Path(file).stem] = df
            print(file)

    def transform(self, cached=True):
        p = os.path.join(self.folder, "log_weekly.json")
        if (not cached) or (not os.path.isfile(p)):
            UserLog = Log()
            for pat in self.environments:
                ls = self._perEnvironment(pat, self.time_field)
                UserLog.addTracePositional(ls, withData=True, isTab=True,
                                                          withExplicitPayloadMap={"user":pat})
            UserLog.indexing()
            with open(p, "w") as outfile:
                json.dump(UserLog.toJSONObject(), outfile, indent=4)
        return p

    def _perEnvironment(self, envName, timedim):
        x = envName
        logger.info("Performining the continuous analysis for "+x)
        logger.trace("1. Data Pre-Processing")
        EntireTimeLog = asFinalLog(self.environments[x], x, self.class_field, self.replace, self.conversion)
        originalChunks = dict()
        # for _, row in self.patients_raw_data[x].iterrows():
        #     toDict =  dict(row)
        #     time = toDict["fulltime"]
        #     del toDict["fulltime"]
        #     originalChunks[time] = {k: toDict[k] for k in toDict if (not isinstance(toDict[k], float)) or (not isnan(toDict[k])) }
        tmp = EntireTimeLog
        assert len(tmp.traces)==1
        booleans, floats = exploseTimeVariations(EntireTimeLog, self.epsilon, self.maxval, self.ignore, self.time_field)
        for drug in self.toExtendWithTime:
            floats[drug] = [drug, self.time_field]
        EntireTimeLog = EntireTimeLog.EventExtraction(booleans, floats, True)
        for idx, row in enumerate(tmp.traces[0].events):
            row.setValue("__class", row.activityLabel)
            row.activityLabel = "__raw_data"
            # assert row["fulltime"] == row["time"]
            originalChunks[datetime.datetime.fromisoformat(row[self.time_field])] = [row]
        logger.trace("2. Weekly data separation into immediate previous and immediate afterwards+mining")
        ewl_idx = MultiTraceIndexing(EntireTimeLog)
        time_continuous_analysis = ewl_idx.segmentByXTraceEventLabel(x + '@label')
        # comparison = list()
        count = 1
        # classOff = {'__class': 'Off'}
        # classOk = {'__class': 'Ok'}
        # mainStream = []
        # TimeSeriesLogOff = Log()
        # TimeSeriesLogOk = Log()
        for idx, analysis in enumerate(time_continuous_analysis):
            polyL = performMiningOverAnalysedLog(analysis.log, None, self.toExtendWithTime, None, None, timedim)
            for i in range(len(polyL)):
                if len(polyL[i]) > 0:
                    t = min(
                        map(lambda x: datetime.datetime.fromisoformat(str(x.getValue(self.time_field))), polyL[i]))
                    assert all(map(lambda x: datetime.datetime.fromisoformat(str(x.getValue(self.time_field))) == t,
                                   polyL[i]))
                    assert t in originalChunks
                    originalChunks[t][0].setValue("__label", analysis.label)
                    originalChunks[t] = originalChunks[t] + polyL[i]

        return [originalChunks[t] for t in sorted(originalChunks.keys())]

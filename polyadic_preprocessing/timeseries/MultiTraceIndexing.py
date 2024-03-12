import sys

from timeseries.Log import Log, TracePositional

def split_contiguous(l):
    from itertools import groupby
    from operator import itemgetter
    for k, g in groupby(enumerate(l), lambda i_x: i_x[0] - i_x[1]):
        yield list(map(itemgetter(1), g))

class OutcomeAnalysis:
    def __init__(self, label, min, max, log):
        self.label = label
        self.min = min
        self.max = max
        self.log = log
class MultiTraceIndexing():
    def __init__(self, l : Log):
        self.traceNameToId = dict()
        self.N = sys.maxsize
        self.log = l
        for idx, x in enumerate(l.traces):
            assert isinstance(x, TracePositional)
            self.traceNameToId[ x.getTraceName()] = idx
            self.N = min(self.N, len(x))

    def asPolyadicTrace(self, idx=1):
        trace = [list(zip(*map(lambda x: x.events, self.log.traces[idx:])))]
        return trace

    def segmentByXTraceEventLabel(self, x, extendDictionary=None)->list[OutcomeAnalysis]:
        """
        Partition the log into temporally-coherent sub-logs where each trace in a sub-log reflects events happening while another trace x has a specific label
        :param x:   Trace to drive the segmentation
        :return:    Segmented log by sublogs depending on the trace label of the events for x
        """
        pe = dict()
        W = list()
        if x in self.traceNameToId:
            for k,v in self.log.traces[self.traceNameToId[x]].positional_events.items():
                pe[k] = list(v)
        for label in pe:
            for idx_list in split_contiguous(pe[label]):
                oa = OutcomeAnalysis(label, min(idx_list), max(idx_list), MultiTraceIndexing(self.log.selectEvents(idx_list)))
                if extendDictionary is not None:
                    oa = extendDictionary(oa)
                W.append(oa)
        return sorted(W, key=lambda d: d.min)

    def projectProperties(self, key, agg):
        return list(map(lambda i: agg(map(lambda x: x[i][key], self.log.traces)), range(self.N)))

    def subtraceFromInterval(self, key, conv, x):
        return MultiTraceIndexing(self.log.subtraceFromInterval(key, conv, x))

    def hasLogWithEmptyTraces(self):
        return all(map(lambda x : len(x)==0, self.log))

    def groupTraceByLabelCondition(self, fun, t=None):
        d = dict()
        for label in self.traceNameToId:
            res = fun(label)
            if res not in d:
                d[res] = list()
            d[res].append(self.log[self.traceNameToId[label]])
        for res in set(d.keys()):
            if t is not None:
                for log in d[res]:
                    log.setTraceName(t(log.getTraceName()))
            d[res] = MultiTraceIndexing(Log(withData=self.log.withData, log=d[res]))
        return d

    def getTraceByName(self, x):
        if x not in self.traceNameToId:
            return None
        else:
            return self.log[self.traceNameToId[x]]

    def __len__(self):
        return self.N

    def __getitem__(self, item):
        result = dict()
        for idx, x in enumerate(self.log.traces):
            result[x.getTraceName()] = x.traces[item]
        return result

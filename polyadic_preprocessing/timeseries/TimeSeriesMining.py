import copy
import datetime
import statistics
from collections import OrderedDict, defaultdict

from timeseries.Log import Event, EventPayload
from timeseries.MultiTraceIndexing import split_contiguous
from timeseries.SequentialPatternMining import MiningConfiguration, SequentialPatternMining


class PatternType:
    def __init__(self, start_time, length, action, for_min_max, name=None, composite=None, payloadMapIterable=None):
        self.start_time = start_time
        self.length = length
        self.action = action
        self.name = name
        self.composite = composite
        if payloadMapIterable is None:
            self.payloadMapIterable = list()
        else:
            self.payloadMapIterable = payloadMapIterable
        if for_min_max is not None:
            self.min = for_min_max[0]
            self.max = for_min_max[-1]
        else:
            self.min = None
            self.max = None

    def __eq__(self, other):
        if self.start_time != other.start_time:
            return False
        if self.length != other.length:
            return False
        if self.action != other.action:
            return False
        if self.name != other.name:
            return False
        if self.min != other.min:
            return False
        if self.max != other.max:
            return False
        if isinstance(self.composite, list) and isinstance(other.composite, list):
            if len(self.composite) != len(other.composite):
                return False
            else:
                for (x,y) in zip(self.composite, other.composite):
                    if x != y:
                        return False
                return True
        else:
            return self.composite == other.composite

    def __repr__(self):
        return "|"+self.name+" for '"+self.action+"' @"+str(self.min)+"|="+str(self.length)

    def __str__(self):
        return self.__repr__()

    def getPayloadList(self):
        if self.payloadMapIterable is None:
            self.payloadMapIterable = list()
        elif not isinstance(self.payloadMapIterable, list):
            self.payloadMapIterable = list(filter(lambda x: len(x)>0, self.payloadMapIterable))
        return self.payloadMapIterable

    def getEventPayloadKeys(self):
        keyUnion = set()
        if self.composite is not None:
            for x in self.composite:
                keyUnion = keyUnion.union(x.getEventPayloadKeys())
        for x in self.getPayloadList():
            for k in x:
                keyUnion.add(k)
        return keyUnion

    def collectValuesForKey(self, k):
        ls = []
        for x in self.getPayloadList():
            if k in x:
                try:
                    ls.append(float(x[k]))
                except ValueError:
                    pass
        if self.composite is not None:
            for x in self.composite:
                ls.extend(x.collectValuesForKey(k))
        return ls

    def collectStringsForKey(self, k):
        ls = []
        for x in self.getPayloadList():
            if k in x:
                ls.append(x[k])
        if self.composite is not None:
            for x in self.composite:
                ls.extend(x.collectStringsForKey(k))
        return ls


    def asTraceEvent(self, d):
            row = dict()
            row['span'] = self.length
            row['time'] = min(map(datetime.datetime.fromisoformat, self.collectStringsForKey('time'))).isoformat()
            action = self.action
            if action.startswith("N") or action.startswith("Y"):
                action = action[1:]
            label = self.name+"("+action+")"
            values = dict()
            for x in self.getEventPayloadKeys():
                values[x] = self.collectValuesForKey(x)
            for k,v in values.items():
                if (len(v)>1):
                    row[k+"_max"] = max(v)
                    row[k+"_min"] = min(v)
                    row[k+"_mean"] =sum(v)/len(v)
                    row[k+"_var"] = statistics.variance(v)
                    row[k+"_med"] = statistics.median(v)
                    row[k+"_stdev"] = statistics.stdev(v)
                elif (len(v)==1):
                    row[k]=v[0]
            d[self.start_time].append(Event(label, EventPayload(row.items())))
            return d

    @staticmethod
    def fromConstituentPattern(name, action, composite):
        return PatternType(composite.start_time,
                           composite.length,
                           action,
                           [composite.min,composite.max],
                           name,
                           [composite])

    @staticmethod
    def fromConstituentPatternList(name, action, composite):
        return PatternType(min(map(lambda x: x.start_time, composite)),
                           sum(map(lambda x: x.length, composite)),
                           action,
                           [min(map(lambda x: x.min, composite)),
                                       max(map(lambda x : x.max, composite))],
                           name,
                           composite)
def sublists(lst):
    sublists_list = []
    for i in range(len(lst)):
        for j in range(i + 1, len(lst) + 1):
            sublists_list.append(lst[i:j])
    return sublists_list

def sub_mining(action,
               maxLen,
               currPattern,
               opposingPattern,
               constantly_ok,
               one_hiccup,
               two_hiccups,
               one_hiccup_next,
               two_hiccups_next,
               volatile_one,
               volatile_two,
               volatile_cmp,
               peFinal):
    if currPattern is None:
        return peFinal
    volatileOneComposition = dict()
    for start_time, d in currPattern.items():
        prev_time = start_time-1
        prev_prev_time = prev_time-1
        next_time = start_time+1
        next_next_time = next_time+1
        for length, ls in d.items():
            if length>1:
                for x in ls:
                    if start_time not in peFinal:
                        peFinal[start_time] = dict()
                    if length not in peFinal[start_time]:
                        peFinal[start_time][length] = list()
                    patt = PatternType.fromConstituentPattern(constantly_ok, action, x)
                    # assert patt.length == length
                    # assert patt.min == start_time
                    peFinal[start_time][length].append(patt)
                    if opposingPattern is not None:
                        for prev_start_time in range(x.min):
                            if prev_start_time in opposingPattern:
                                lengthPrev = x.min - prev_start_time
                                if lengthPrev in opposingPattern[prev_start_time]:
                                    for y in opposingPattern[prev_start_time][lengthPrev]:
                                        if prev_start_time not in peFinal:
                                            peFinal[prev_start_time] = dict()
                                        if lengthPrev+x.length not in peFinal[prev_start_time]:
                                            peFinal[prev_start_time][lengthPrev+x.length] = list()
                                        if prev_start_time not in volatileOneComposition:
                                            volatileOneComposition[prev_start_time]  = dict()
                                        if lengthPrev+x.length not in volatileOneComposition[prev_start_time]:
                                            volatileOneComposition[prev_start_time][lengthPrev+x.length] = list()
                                        patt = PatternType.fromConstituentPatternList(volatile_one, action, [y, x])
                                        # assert patt.length == lengthPrev+x.length
                                        # assert patt.min == prev_start_time
                                        peFinal[prev_start_time][lengthPrev+x.length].append(patt)
                                        volatileOneComposition[prev_start_time][lengthPrev+x.length].append(patt)
                    if opposingPattern is not None:
                        if prev_time > 0:
                            if prev_time in opposingPattern:
                                if 1 in opposingPattern[prev_time]:
                                    for y in opposingPattern[prev_time][1]:
                                        if prev_time not in peFinal:
                                            peFinal[prev_time] = dict()
                                        if length+1 not in peFinal[prev_time]:
                                            peFinal[prev_time][length+1] = list()
                                        patt =  PatternType.fromConstituentPatternList(one_hiccup,action,  [y,x])
                                        # assert patt.length == length+1
                                        # assert patt.min == prev_time
                                        peFinal[prev_time][length+1].append(patt)
                                        if next_time < maxLen:
                                            if next_time in opposingPattern:
                                                if 1 in opposingPattern[next_time]:
                                                    for z in opposingPattern[next_time][1]:
                                                        if length + 2 not in peFinal[prev_time]:
                                                            peFinal[prev_time][length + 2] = list()
                                                        patt = PatternType.fromConstituentPatternList(volatile_two, action, [y,x,z])
                                                        # assert patt.length == length + 2
                                                        # assert patt.min == prev_time
                                                        peFinal[prev_time][length + 2].append(patt)
                                        if prev_prev_time > 0:
                                            if prev_prev_time in currPattern:
                                                if 1 in currPattern[prev_prev_time]:
                                                    for z in currPattern[prev_prev_time][1]:
                                                        if prev_prev_time not in peFinal:
                                                            peFinal[prev_prev_time] = dict()
                                                        if length + 2 not in peFinal[prev_prev_time]:
                                                            peFinal[prev_prev_time][length + 2] = list()
                                                        patt = PatternType.fromConstituentPatternList(two_hiccups,action,
                                                                                                   [z, y, x])
                                                        # assert patt.length == length + 2
                                                        # assert patt.min == prev_prev_time
                                                        peFinal[prev_prev_time][length+2].append(patt)
                        if next_time < maxLen:
                            if next_time in opposingPattern:
                                if 1 in opposingPattern[next_time]:
                                    for y in opposingPattern[next_time][1]:
                                        if length+1 not in peFinal[start_time]:
                                            peFinal[start_time][length+1] = list()
                                        patt = PatternType.fromConstituentPatternList(one_hiccup_next,action,  [x,y])
                                        # assert patt.length == length + 1
                                        # assert patt.min == start_time
                                        peFinal[start_time][length+1].append(patt)
                                        if next_next_time < maxLen:
                                            if next_next_time in currPattern:
                                                if 1 in currPattern[next_next_time]:
                                                    for z in currPattern[next_next_time][1]:
                                                        if length + 2 not in peFinal[start_time]:
                                                            peFinal[start_time][length + 2] = list()
                                                        patt = PatternType.fromConstituentPatternList(two_hiccups_next,action,
                                                                                                   [x,y,z])
                                                        # assert patt.length == length + 2
                                                        # assert patt.min == start_time
                                                        peFinal[start_time][length+2].append(patt)
    if len(volatileOneComposition)> 0:
        skipKey = min(volatileOneComposition.keys())
        for startTime in sorted(list(volatileOneComposition.keys())):
            if startTime == skipKey:
                continue
            for ll, vals in volatileOneComposition[startTime].items():
                for x in vals:
                    for prev_start_time in range(x.min):
                        if prev_start_time in volatileOneComposition:
                            lengthPrev = x.min - prev_start_time
                            if lengthPrev in volatileOneComposition[prev_start_time]:
                                for y in volatileOneComposition[prev_start_time][lengthPrev]:
                                    if prev_start_time not in peFinal:
                                        peFinal[prev_start_time] = dict()
                                    if lengthPrev+x.length not in peFinal[prev_start_time]:
                                        peFinal[prev_start_time][lengthPrev+x.length] = list()
                                    patt = PatternType.fromConstituentPatternList(volatile_cmp, action, [y, x])
                                    # assert patt.length == lengthPrev + x.length
                                    # assert patt.min == prev_start_time
                                    peFinal[prev_start_time][lengthPrev + x.length].append(patt)
    return peFinal


def rec_visit(peFinal, v, ls, result):
    for x in v:
        L = copy.copy(ls)
        L.append(x)
        if x.min+x.length in peFinal:
            for len2, w  in peFinal[x.min+x.length].items():
                rec_visit(peFinal, w, L, result)
        else:
            result.append(L)




def mine_binary_growth_patterns(trace, maxLen, pos, neg, actione, conf, L):
    pe = {pos:None, neg:None}
    ## Determining the simple patterns
    for action, v in trace.positional_events.items():
        pad = dict()
        for contiguous in split_contiguous(v):
            for sList in sublists(contiguous):
                if len(sList)>0:
                    start_time = sList[0]
                    length = len(sList)
                    action = action
                    payloadMapIterable = map(lambda x: trace[x].eventpayload.trace_data, sList)
                    pt = PatternType(start_time, length, action, sList, payloadMapIterable=payloadMapIterable)
                    if start_time not in pad:
                        pad[start_time] = dict()
                    if length not in pad[start_time]:
                        pad[start_time][length] = list()
                    pad[start_time][length].append(pt)
        pe[action] = pad

    # assert pos in pe
    # assert neg in pe
    posPattern = pe[pos]
    negPattern = pe[neg]

    peFinal = sub_mining(actione, maxLen,
               posPattern,
               negPattern,
               "IncreaseRapidly",
               "IncreaseSlowlyI",
               "IncreaseSlowlyII",
               "IncreaseSlowlyIII",
               "IncreaseSlowlyIV",
                         "HighVolatilityI",
                         "HighVolatilityIII",
                         "HighVolatilityII",
               dict())
    peFinal = sub_mining(actione, maxLen,
               negPattern,
               posPattern,
               "DecreaseRapidly",
               "DecreaseSlowlyIV",
               "DecreaseSlowlyIII",
               "DecreaseSlowlyII",
               "DecreaseSlowlyI",
                         "HighVolatilityVI",
                         "HighVolatilityIV",
                         "HighVolatilityV",
               peFinal)

    L.addMinedDictionaryByActionType(peFinal)
    # algo = None
    # if conf.doMining:
    #     # multiset = dict()
    #     result = []
    #     if conf.algorithm.startswith("polyadic_"):
    #         none = "."
    #         l = [set() for i in range(maxLen)]
    #         for start_time, d in peFinal.items():
    #             for length, v in d.items():
    #                 for x in v:
    #                     l[start_time].add(x.name)
    #         l = list(map(lambda x: list(x), l))
    #         algo = l
    #     elif conf.algorithm != "episodical":
    #         linearised = []
    #         db = []
    #         for start_time, d in peFinal.items():
    #             for length, v in d.items():
    #                 rec_visit(peFinal, v, list(), linearised)
    #         # if compact:
    #         #     db = list(map(lambda x: list(x), set([tuple(map(lambda y: y.name, x)) for x in linearised])))
    #         # else:
    #         db = [list(map(lambda y: y.name, x)) for x in linearised]
    #         algo = SequentialPatternMining(db)
    #     else:
    #         conf.maxLen = maxLen
    #         algo = SequentialPatternMining(peFinal)
    # return algo









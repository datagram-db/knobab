import copy
import datetime
import itertools
import statistics
from collections import OrderedDict, defaultdict

from EMeriTAte.timeseries.Log import Event, EventPayload
from EMeriTAte.timeseries.MultiTraceIndexing import split_contiguous
from EMeriTAte.timeseries.SequentialPatternMining import MiningConfiguration, SequentialPatternMining


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

    def __hash__(self):
        return hash((self.name, self.length, self.max, self.min, self.action, self.start_time))

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


    def asTraceEvent(self, d, timefield):
            row = dict()
            row['span'] = self.length
            try:
                row[timefield] = str(min(map(int, self.collectStringsForKey(timefield))))
            except:
                row[timefield] = datetime.datetime.fromisoformat(min(map(datetime.datetime.fromisoformat, self.collectStringsForKey(timefield))).isoformat())
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
    for i in range(len(lst)):
        for j in range(i + 1, len(lst) + 1):
            yield map(lambda x: lst[x], range(i,min(j,len(lst)))), j-i, lst[i], lst[j-1]

def extend(L1, L2):
    L1[0:0] = L2
    return L1

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
    volatileOneComposition = {t: defaultdict(list) for t in peFinal}
    for start_time, d in currPattern.items():
        prev_time = start_time-1
        prev_prev_time = prev_time-1
        next_time = start_time+1
        next_next_time = next_time+1
        for length, ls in d.items():
            if length>1:
                peFinal[start_time][length] = extend(peFinal[start_time][length], [PatternType.fromConstituentPattern(constantly_ok, action, x) for x in ls])
                if opposingPattern is None:
                    continue
                for x in ls:
                        for prev_start_time in range(x.min):
                                lengthPrev = x.min - prev_start_time
                                LLL = [PatternType.fromConstituentPatternList(volatile_one, action, [y, x]) for y in opposingPattern[prev_start_time][lengthPrev]]
                                peFinal[prev_start_time][lengthPrev+x.length] = extend(peFinal[prev_start_time][lengthPrev+x.length], LLL)
                                volatileOneComposition[prev_start_time][lengthPrev+x.length] = extend(volatileOneComposition[prev_start_time][lengthPrev+x.length], LLL)  #.append(patt2)
                        if prev_time > 0:
                                    for y in opposingPattern[prev_time][1]:
                                        patt =  PatternType.fromConstituentPatternList(one_hiccup,action,  [y,x])
                                        peFinal[prev_time][length+1] = extend(peFinal[prev_time][length+1], [patt])
                                        if next_time < maxLen:
                                            if next_time in opposingPattern:
                                                peFinal[prev_time][length + 2] = extend(peFinal[prev_time][length + 2], map(lambda z: PatternType.fromConstituentPatternList(volatile_two, action, [y,x,z]), opposingPattern[next_time][1]))
                                        if prev_prev_time > 0:
                                            if prev_prev_time in currPattern:
                                                peFinal[prev_prev_time][length + 2] = extend(peFinal[prev_prev_time][length + 2], map(lambda z: PatternType.fromConstituentPatternList(two_hiccups,action,
                                                                                                   [z, y, x]), currPattern[prev_prev_time][1]))
                        if next_time < maxLen:
                            if next_time in opposingPattern:
                                    # if 1 in opposingPattern[next_time]:
                                    for y in opposingPattern[next_time][1]:
                                        patt = PatternType.fromConstituentPatternList(one_hiccup_next,action,  [x,y])
                                        peFinal[start_time][length+1] = extend(peFinal[start_time][length+1], [patt])
                                        if next_next_time < maxLen:
                                                peFinal[start_time][length + 2] = extend(peFinal[start_time][length + 2], map(lambda z: PatternType.fromConstituentPatternList(two_hiccups_next,action,
                                                                                                   [x,y,z]), currPattern[next_next_time][1]))
    if len(volatileOneComposition)> 0:
        skipKey = min(volatileOneComposition.keys())
        for startTime in sorted(list(volatileOneComposition.keys())):
            if startTime == skipKey:
                continue
            for ll, vals in volatileOneComposition[startTime].items():
                for x in vals:
                    for prev_start_time in set(volatileOneComposition.keys()).intersection(range(x.min)):
                            lengthPrev = x.min - prev_start_time
                            peFinal[prev_start_time][lengthPrev + x.length] = extend(peFinal[prev_start_time][lengthPrev + x.length], map(lambda y: PatternType.fromConstituentPatternList(volatile_cmp, action, [y, x]), volatileOneComposition[prev_start_time][lengthPrev]))
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




def mine_binary_growth_patterns(trace, maxLen, pos, neg, actione, L):
    pe = {pos:None, neg:None}
    ## Determining the simple patterns
    # print("Determining the simple patterns")
    for action, v in trace.positional_events.items():
        pad = {t:defaultdict(list) for t in range(trace.length)}
        for contiguous in split_contiguous(v):
            for sList, length, start_time, end_time in sublists(contiguous):
                    pt = PatternType(start_time, length, action, [start_time,end_time], payloadMapIterable=map(lambda x: {} if trace[x].eventpayload is None else trace[x].eventpayload.trace_data, sList))
                    pad[start_time][length].append(pt)
        pe[action] = pad

    # assert pos in pe
    # assert neg in pe
    posPattern = pe[pos]
    negPattern = pe[neg]

    # print("sub_mining 1")
    if posPattern is None and negPattern is None:
        return
    elif posPattern is None:
        S = negPattern.keys()
    elif negPattern is None:
        S = posPattern.keys()
    else:
        S = set(posPattern.keys()).union(set(negPattern.keys()))

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
                         None if posPattern is None else {k: defaultdict(list) for k in S})

    # print("sub_mining 2")
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
               peFinal if peFinal is not None else {k: defaultdict(list) for k in S})

    # print("addMinedDictionaryByActionType")
    L.addMinedDictionaryByActionType(peFinal)









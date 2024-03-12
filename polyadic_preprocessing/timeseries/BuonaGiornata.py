from sortedcontainers import SortedDict

def meteorology(HV1, IS1, sortedActionSet, lenX):
    HVR = []
    if HV1 in sortedActionSet and IS1 in sortedActionSet:
        for idx in range(lenX):
            if len(sortedActionSet[HV1][idx]) > 0 and len(sortedActionSet[IS1][idx]) > 0:
                assert len(sortedActionSet[HV1][idx]) == 1
                assert len(sortedActionSet[IS1][idx]) == 1
                if all(map(lambda z: z[0] == z[1], zip(sortedActionSet[HV1][idx][0].composite,
                                                       sortedActionSet[IS1][idx][0].composite))):
                    HVR.append(idx)
    return HVR

class Indexing:
    def asListForTraces(self, L):
        for entry in self.sortedActionSet.values():
            for ls in entry:
                for x in ls:
                    L = x.asTraceEvent(L)
        return L

    def __init__(self, lenX, start_time, fromBuonaGiornataAndKey):
        self.start_time = start_time
        self.len = lenX
        self.actionSet = set()
        HV1 = "HighVolatilityI"
        IS1 = "IncreaseSlowlyI"
        HV2 = "HighVolatilityVI"
        DS2 = "DecreaseSlowlyIV"
        for start_time, d1 in fromBuonaGiornataAndKey.items():
            for length, ls in d1.items():
                for pt in ls:
                    self.actionSet.add(pt.name)
        self.sortedActionSet = {k: [[] for _ in range(self.len)] for k in self.actionSet}
        for start_time, d1 in fromBuonaGiornataAndKey.items():
            for length, ls in d1.items():
                for pt in ls:
                    self.sortedActionSet[pt.name][start_time].append(pt)

        ## Optimizations
        ## The subsequent set of refinements is required to discard redundant events and correlations that might appear

        for pt_name in self.actionSet:
            for idx in range(self.len):
                if len(self.sortedActionSet[pt_name][idx])>0:
                    ## 1. First refinement: across all the elements associated to the same action name, keeping just the
                    ##    most long-lasting one, and discarding all the others, as this preceeding event will still
                    ##    correlate in the future with any other one further occurring
                    maxLengthFromCurrentTime = max(map(lambda x: x.length, self.sortedActionSet[pt_name][idx]))
                    self.sortedActionSet[pt_name][idx] = list(filter(lambda x: x.length == maxLengthFromCurrentTime, self.sortedActionSet[pt_name][idx]))
                    for j in range(idx+1, idx+maxLengthFromCurrentTime, 1):
                        self.sortedActionSet[pt_name][j].clear()

        ## 2. Second refinement: across all the elements having both HV1&IS1 and HV2&DS2 at the same running time,
        ##    keep the most specific one (IS1 or DS2) of the two if the constituents for both are the same (as they might
        ##    describe the same patter, we just prefer one among the two).
        for j in meteorology(HV1, IS1, self.sortedActionSet, self.len):
            self.sortedActionSet[HV1][j].clear()
        for j in meteorology(HV2, DS2, self.sortedActionSet, self.len):
            self.sortedActionSet[HV2][j].clear()



class BuonaGiornata:
    def __init__(self, toDiscard=None):
        self.d = dict()
        self.len = 0
        self.start_time = 0
        if toDiscard is None:
            self.toDiscard = lambda x: False
        else:
            self.toDiscard = toDiscard

    def finalise(self):
        L = [list() for _ in range(self.len)]
        for k in self.d:
            idx = Indexing(self.len, self.start_time, self.d[k])
            L = idx.asListForTraces(L)
            self.d[k] = idx
        return L

    def addMinedDictionaryByActionType(self, extDict):
        for start_time, d1 in extDict.items():
            for length, ls in d1.items():
                for pt in ls:
                    self.addPattern(start_time, length, pt)
    def addPattern(self, start_time, length, pt, selector=None):
        if selector is None:
            selector = pt.action
        else:
            selector = selector(pt)
        if self.toDiscard(selector):
            return
        self.start_time = max(self.start_time, pt.start_time)
        self.len = max(self.len, pt.start_time+pt.length)
        if selector not in self.d:
            ls = [pt]
            dl = SortedDict({length: ls})
            dt = SortedDict({start_time: dl})
            self.d[selector] = dt
        elif start_time not in self.d[selector]:
            ls = [pt]
            dl = SortedDict({length: ls})
            self.d[selector][start_time] = dl
        elif length not in self.d[selector][start_time]:
            self.d[selector][start_time][length] = [pt]
        else:
            self.d[selector][start_time][length].append(pt)




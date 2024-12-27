import functools
import sys
from collections import defaultdict
from dataclasses import dataclass
from typing import List

import numpy
import pandas
from big_o import infer_big_o_class

from SortedIntervalTrees import Group, SortedIntervalTrees

import itertools
def pairwise(iterable):
    """s -> (s0, s1), (s1, s2), (s2, s3), ..."""
    a, b = itertools.tee(iterable)
    next(b, None)
    return zip(a, b)

@dataclass
class BasicRecord:
    type:          bool  # The former Y/N
    time_spot:     int   # The pointwise event's position in the trace
    raw_payload:   float # The payload associated to the position name
    additional:    float # Whether it has additional payload from a different type of event capturing

def absence_gen(epsilon, idx, value):
    """
    Generating an absence event
    @param epsilon:
    @param idx:
    @param value:
    @return:
    """
    abs_val = abs(value)
    absence_cond = abs_val <= epsilon
    return BasicRecord(absence_cond, idx, value, abs_val if not absence_cond else 0.0)

def increase_gen(idx, curr, next):
    """
    Generating an increase event
    @param idx:
    @param curr:
    @param next:
    @return:
    """
    diff = next-curr
    absence_cond = diff>0
    return BasicRecord(absence_cond, idx, next, diff)

def stationariety_gen(epsilon, idx, curr, next):
    """
    Generating a stationary event
    @param epsilon:
    @param idx:
    @param curr:
    @param next:
    @return:
    """
    diff = abs(next - curr)
    stationariety_cond = diff <= epsilon
    return BasicRecord(stationariety_cond, idx, next, diff if not stationariety_cond else 0.0)

def variation_gen(epsilon, maxval, idx, curr, next):
    """
    Generating a variation event
    @param epsilon:
    @param maxval:
    @param idx:
    @param curr:
    @param next:
    @return:
    """
    diff = abs(next - curr)
    stationariety_cond = diff <= epsilon
    if stationariety_cond:
        return BasicRecord(stationariety_cond, idx, next, 0.0)
    elif abs(curr) <= epsilon:
        return BasicRecord(not stationariety_cond, idx, next, maxval)
    else:
        return BasicRecord(not stationariety_cond, idx, next, (next - curr) / curr)

def transform_series(dim:str, ls:List[float], epsilon=0.0001, maxval=sys.float_info.max)->List[BasicRecord]:
    """
    This method mimicks the old Bepi.defineIntervals
    @param dim:
    @param ls:
    @param epsilon:
    @param maxval:
    @return:
    """
    increase_list = [None] * len(ls)
    absence_list = [None] * len(ls)
    stationary_list = [None] * len(ls)
    variability_list = [None] * len(ls)
    for idx, (curr, next) in enumerate(pairwise(ls)):
        if idx == 0:
            increase_list[0] = BasicRecord(True, idx, curr, curr)
            absence_cond = abs(curr) <= epsilon
            absence_list[0] = BasicRecord(absence_cond, idx, curr, abs(curr) if not absence_cond else 0.0)
            stationary_list[0] = BasicRecord(False, idx, curr, curr)
            variability_list[0] = BasicRecord(True, idx, curr, maxval)
        increase_list[idx+1] =                 increase_gen(idx+1, curr, next)
        absence_list[idx+1] =          absence_gen(epsilon, idx+1, next)
        stationary_list[idx+1] = stationariety_gen(epsilon, idx, curr, next)
        variability_list[idx+1] = variation_gen(epsilon, maxval, idx, curr, next)
    increase_list = RecordHandling(dim+"_i", increase_list)
    absence_list = RecordHandling(dim+"_a", absence_list)
    stationary_list = RecordHandling(dim+"_s", stationary_list)
    variability_list = RecordHandling(dim+"_v", variability_list)
    return increase_list, absence_list, stationary_list, variability_list

# def merge_iterables(*iterables):
#     for x in iterables:
#         yield from x

class RecordHandling:
    """
    This class is the one responsible for now generating the DT patterns efficiently without getting bonkers
    """
    def __init__(self, dimension:str, ls:List[BasicRecord]):
        """
        This starts by splitting the elements between different groups, of positive and negative events
        @param dimension:
        @param ls:
        """
        self.groups = [[],[]]
        self.arrow_of_time = []
        self.linear_time = [None] * len(ls)
        self.ls = ls
        self.dimension = dimension
        latest_value = None
        init_curr_value = None
        x = None
        timestamp = None
        for timestamp, x in enumerate(ls):
            if x.type != latest_value:
                if latest_value is None:
                    init_curr_value = timestamp
                    latest_value = x.type
                else:
                    idx = 1 if latest_value else 0
                    aotoff = len(self.arrow_of_time)
                    self.arrow_of_time.append((idx, len(self.groups[idx])))
                    g = Group(init_curr_value, timestamp-1, timestamp-init_curr_value)
                    self.groups[idx].append(g)
                    for prev_times in g.asRange():
                        self.linear_time[prev_times] = aotoff
                    init_curr_value = timestamp
                    latest_value = x.type
        if len(ls) > 0:
            idx = 1 if x.type else 0
            aotoff = len(self.arrow_of_time)
            self.arrow_of_time.append((idx, len(self.groups[idx])))
            g = Group(init_curr_value, timestamp, timestamp-init_curr_value+1)
            self.groups[1 if x.type else 0].append(g)
            for prev_times in g.asRange():
                self.linear_time[prev_times] = aotoff
        self.groups[0] = SortedIntervalTrees(self.groups[0])
        self.groups[1] = SortedIntervalTrees(self.groups[1])
        self.straight = ["DecreaseRapidly","IncreaseRapidly"]
        self.V1      = ["HighVolatility6","HighVolatility1"]
        self.V2      = ["HighVolatility4","HighVolatility3"]
        self.Vcmp      = ["HighVolatility5","HighVolatility2"]
        self._1H      = ["DecreaseSlowly4","IncreaseSlowly1"]
        self._2H      = ["DecreaseSlowly3","IncreaseSlowly2"]
        self._1HN      = ["DecreaseSlowly2","IncreaseSlowly3"]
        self._2HN      = ["DecreaseSlowly1","IncreaseSlowly4"]

    def hasPositiveEvent(self, idx):
        return self.groups[1].inIntervalTree(idx)

    def hasNegativeEvent(self, idx):
        return self.groups[0].inIntervalTree(idx)

    def genPureInterval(self, interval_type, begin, end):
        from EMeriTAte.timeseries.MyCatch24 import my_catch24
        from EMeriTAte.timeseries.Log import EventPayload, Event
        import statistics
        act = interval_type+"("+self.dimension+")"
        span = end+1-begin
        time_spot = [None] * span
        payload__ = [None] * span
        for idx in range(span):
            obj = self.ls[begin+idx]
            time_spot[idx] = obj.time_spot
            payload__[idx] = obj.raw_payload
        payload = dict()
        payload["span"] = span
        my_catch24(self.dimension, payload__, payload)
        my_catch24("time", time_spot, payload)
        return Event(act, EventPayload(payload))

    def pushConcurrentIntervalInterval(self, executor, interval_type, begin, end):
        from EMeriTAte.timeseries.MyCatch24 import my_catch24
        from EMeriTAte.timeseries.Log import EventPayload, Event
        import statistics
        act = interval_type+"("+self.dimension+")"
        span = end+1-begin
        time_spot = [None] * span
        payload__ = [None] * span
        for idx in range(span):
            obj = self.ls[begin+idx]
            time_spot[idx] = obj.time_spot
            payload__[idx] = obj.raw_payload
        payload = dict()
        payload["span"] = span
        my_catch24(self.dimension, payload__, payload)
        my_catch24("time", time_spot, payload)
        return Event(act, EventPayload(payload))

    def genInterval(self, interval_type, begin, end):
        from EMeriTAte.timeseries.MyCatch24 import my_catch24
        from EMeriTAte.timeseries.Log import EventPayload, Event
        import statistics
        act = interval_type+"("+self.dimension+")"
        span = end+1-begin
        # time_spot = [None] * span
        payload__ = [(self.ls[begin+idx]).raw_payload for idx in range(span)]
        # for idx in range(span):
        #     obj = self.ls[begin+idx]
        #     # time_spot[idx] = obj.time_spot
        #     payload__[idx] = obj.raw_payload
        payload = dict()
        payload["span"] = span
        my_catch24(self.dimension, payload__, payload)
        # my_catch24("time", time_spot, payload)
        return begin, Event(act, EventPayload(payload))

    def jumpToArrowOfTimeAlgorithm3rdVersion(self,executor, time):
        arrow_idx = self.linear_time[time]
        group_type, group_offset = self.arrow_of_time[arrow_idx]
        groupRange = self.groups[group_type][group_offset]
        ## TODO: the \/\/ pattern out of \/-s
        n = len(self.arrow_of_time)
        # prev_type = None
        # next = None
        flip = (group_type + 1) % 2
        print(time)
        for end_time in range(time, groupRange.end + 1):
            span = end_time - time + 1
            if span > 1:
                yield self.genPureInterval(self.straight[group_type], time,
                                           end_time)  ## Generating the recto verso for the curren tinterval
            if ((arrow_idx + 1) < n) and (end_time == groupRange.end):
                next_ref = self.arrow_of_time[arrow_idx + 1]
                next = self.groups[flip][next_ref[1]]
                if span == 1:
                    ## H1 + V2/2H
                    for next_time in next.asRangeSkipFirst():
                        yield self.genPureInterval(self._1H[flip], time, next_time)
                    if (arrow_idx + 2) < n:
                        if len(next) > 1:
                            yield self.genPureInterval(self.V2[flip], time, next.end + 1)
                        else:
                            nextnext = self.groups[flip][next_ref[1]]
                            for nextnext_time in nextnext.asRangeSkipFirst():
                                yield self.genPureInterval(self._2H[group_type], time, nextnext_time)
                else:
                    yield self.genPureInterval(self._1HN[group_type], time, groupRange.end + 1)
                    if (len(next) == 1) and ((arrow_idx + 2) < n):
                        yield self.genPureInterval(self._2HN[group_type], time, groupRange.end + 2)

    def _GenerationAlgorithm2ndVersion(self, x: Group, group_type):
        """
        This algorithm is a speedup if compared to the original IDEAS'24 paper, as we are not required to get the data
        @param x:           The group from which we want to generate data
        @param group_type:  The type associated with the group
        @return:            An iterable of events, not being sorted by time
        """
        begin_match = defaultdict(set)

        flip = (group_type+1) % 2
        for current_span in range(x.span):
            print(current_span)
            current_span = current_span + 1
            for start in x.asRange(current_span):
                if (start + current_span-1) > x.end:
                    continue
                yield self.genInterval(self.straight[group_type], start, start + current_span - 1)
                Inext = None
                if start == x.end:
                    Inext = self.groups[flip].inIntervalTree(x.end + 1)
                if start == x.start:
                    Iprev = self.groups[flip].inIntervalTree(start - 1)
                    if Iprev is not None:
                        yield self.genInterval(self._1H[group_type], start-1, start + current_span - 1)
                        if Inext is not None:
                            yield self.genInterval(self.V2[group_type], start - 1, start + current_span)
                        if Iprev.span == 1:
                            Iprevprev = self.groups[group_type].inIntervalTree(start-2)
                            if Iprevprev is not None:
                                yield self.genInterval(self._2H[group_type], start - 2, start + current_span-1)
                        else:
                            for prev_start in Iprev.asRange():
                                if prev_start == start-1:
                                    continue
                                yield self.genInterval(self.V1[group_type], prev_start, start + current_span-1)
                                begin_match[prev_start].add((start + current_span-1))
                if (start + current_span - 1 == x.end) and (Inext is not None):
                    yield self.genInterval(self._1HN[group_type], start, start + current_span - 1)
                    if (Inext.span == 1):
                        Inextnext = self.groups[group_type].inIntervalTree(x.end + 2)
                        if Inextnext is not None:
                            yield self.genInterval(self._2HN[group_type], start, start + current_span)
        for begin, ends in begin_match.items():
            for end in ends:
                if end+1 in begin_match:
                    for new_end_time in begin_match[end+1]:
                        yield self.genInterval(self.Vcmp[group_type], begin, new_end_time)


    @staticmethod
    def GenerationAlgorithm(tsL, d, version = None):
        """
        This preliminary version requires you to collect the data, and to then re-sort it within the trace by starting time,
        otherwise we cannot serialise properly into a polyadic trace. With the next set of algorithm we try to further
        generalise into this by guaranteeing to generate events that always start from a specific point in time, thus
        allowing us to avoid the grouping of the events per type

        @param tsL: A list of multidimensional time series, where each dimension is just a number in this case
        @return:
        """
        epsilon = 0.0001
        maxval = sys.float_info.max
        import datetime
        if version == None:
            version = "second"
        time_per_trace = 0
        import concurrent.futures
        with concurrent.futures.ThreadPoolExecutor(max_workers=5) as executor:
            for trace_id, tssL in enumerate(tsL):
                for dim_id, ts in enumerate(tssL):
                    increase_list, absence_list, stationary_list, variability_list = transform_series(f"dim_{dim_id}", ts, epsilon,
                                                                                                      maxval)
                    if version == "second":
                        a = datetime.datetime.now()
                        trace = [list() for _ in range(len(ts))]
                        tot = 0
                        for start_time, x in increase_list.genAllIntervalsAlgorithm2ndVersion():
                            trace[start_time].append(x)
                        for start_time, x in absence_list.genAllIntervalsAlgorithm2ndVersion():
                            trace[start_time].append(x)
                        for start_time, x in stationary_list.genAllIntervalsAlgorithm2ndVersion():
                            trace[start_time].append(x)
                        for start_time, x in variability_list.genAllIntervalsAlgorithm2ndVersion():
                            trace[start_time].append(x)
                        for time, constituents in enumerate(trace):
                            tot += len(constituents)
                            yield trace_id, dim_id, time, constituents
                        b = datetime.datetime.now()
                        c = b - a
                        d[0].append(len(ts))
                        d[1].append(c.total_seconds())
                        print(c.total_seconds())
                    else:
                        a = datetime.datetime.now()
                        # tot = 0
                        for time in range(len(ts)):
                            myset = []
                            myset.extend(increase_list.jumpToArrowOfTimeAlgorithm3rdVersion(executor,time))
                            myset.extend(absence_list.jumpToArrowOfTimeAlgorithm3rdVersion(executor,time))
                            myset.extend(stationary_list.jumpToArrowOfTimeAlgorithm3rdVersion(executor,time))
                            myset.extend(variability_list.jumpToArrowOfTimeAlgorithm3rdVersion(executor,time))
                            # tot += (len(myset))
                            yield trace_id, dim_id, time, myset
                        b = datetime.datetime.now()
                        c = b - a
                        d[0].append(len(ts))
                        d[1].append(c.total_seconds())
                        print(c.total_seconds())







    def genAllIntervalsAlgorithm2ndVersion(self):
        group_type = 1
        for group in self.groups[group_type]:
            yield from self._GenerationAlgorithm2ndVersion(group, group_type)
        group_type = 0
        for group in self.groups[group_type]:
            yield from self._GenerationAlgorithm2ndVersion(group, group_type)
        # if (len(self.groups[1]) == 0) or (len(self.groups[0]) == 0):
        #     yield from []



# import big_o
# positive_int_generator = lambda n: big_o.datagen.n_(n)
#
# def benchmark_algo2(traceLength):
#     dim_name = "dim_0"
#
#     from numpy.random import Generator, MT19937, SeedSequence
#     sg = SeedSequence(1234)
#     nEnvironments = 10
#     tsL = [[Generator(MT19937(a)).normal(0, 1, traceLength)] for a in sg.spawn(nEnvironments)]
#     count = 0
#     tt3 = defaultdict(set)
#     tt2 = defaultdict(set)
#     # tsL = [[[0.0, 0.1, 0.2, 0.3, 1.0, 2.0, 1.9, 1.8, 1.0, 0.9, 1.0, 1.1, 1.0, 0.5, 0.6, 0.7, 0.8, 0.7, 0.8, 0.0]]]##
#     for trace_id, dim_id, time, constituents in RecordHandling.GenerationAlgorithm(tsL):
#         # tt3[time] =  constituents
#         count += len(constituents)
#
# def benchmark_algo3(traceLength):
#     dim_name = "dim_0"
#     epsilon = 0.0001
#     maxval = sys.float_info.max
#     from numpy.random import Generator, MT19937, SeedSequence
#     sg = SeedSequence(1234)
#     nEnvironments = 1
#     tsL = [[Generator(MT19937(a)).normal(0, 1, traceLength)] for a in sg.spawn(nEnvironments)]
#     count = 0
#     tt3 = defaultdict(set)
#     tt2 = defaultdict(set)
#     # tsL = [[[0.0, 0.1, 0.2, 0.3, 1.0, 2.0, 1.9, 1.8, 1.0, 0.9, 1.0, 1.1, 1.0, 0.5, 0.6, 0.7, 0.8, 0.7, 0.8, 0.0]]]##
#     for trace_id, dim_id, time, constituents in RecordHandling.GenerationAlgorithm(tsL, 3):
#         # tt3[time] =  constituents
#         count += len(constituents)

if __name__ == "__main__":
    # best, others = big_o.big_o(benchmark_algo2, big_o.datagen.n_, n_repeats=10, min_n=2, max_n=100,verbose=True)
    # print(best)
    # best, others = big_o.big_o(benchmark_algo3, big_o.datagen.n_, n_repeats=10, min_n=2, max_n=100,verbose=True)
    # print(best)
    dim_name = "dim_0"
    epsilon = 0.0001
    maxval = sys.float_info.max
    from numpy.random import Generator, MT19937, SeedSequence
    sg = SeedSequence(1234)
    nEnvironments = 1
    traceLength = 428
    # print("Generating data...")
    tt3 = [[],[]]
    tt2 = [[],[]]
    for idx in [428]:#range(10):
        idx = (idx+1)*5
        print(idx)
        obj = pandas.read_csv("/home/giacomo/projects/knobab2_loggen/EMeriTAte/osuleaf/0.csv").pop("dim_0")
        # tsL = [[Generator(MT19937(a)).normal(0, 1, idx)] for a in sg.spawn(nEnvironments)]
        count = 0
        # tsL = [[[0.0, 0.1, 0.2, 0.3, 1.0, 2.0, 1.9, 1.8, 1.0, 0.9, 1.0, 1.1, 1.0, 0.5, 0.6, 0.7, 0.8, 0.7, 0.8, 0.0]]]##
        # for trace_id, dim_id, time, constituents in RecordHandling.GenerationAlgorithm(tsL, tt2):
        #     tt3[time] =  constituents
        # count += 1
        # count = 0
        for trace_id, dim_id, time, constituents in RecordHandling.GenerationAlgorithm([[list(obj)]], tt3, "three"):
            # tt2[time] =  constituents
            count += 1

    # t3, _  = infer_big_o_class(tt3[0], tt3[1])
    # t2, _  = infer_big_o_class(tt2[0], tt2[1])
    # print(t2)
    # print(t3)

    # print("Algo2:")
    # for x in tt2[1]:
    #     print(x)
    # print("Algo3:")
    # for x in tt3[1]:
    #     print(x)
    print("DONE")
    # print(tsL)

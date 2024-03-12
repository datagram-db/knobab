import itertools
import math
import sys

from timeseries.Log import TracePositional
def defineIntervals(t:TracePositional,timeField, epsilon=0.0001, maxval=sys.float_info.max, ignorable=None):
    """
    This function provides the discretisation of the time series into increase, variation, and absence events, thus
    better outlining the temporal trends within the numerical data

    :param t:               original trace
    :param timeField:       Payload field related to the time
    :param epsilon:         Value under which we ignore any variation
    :param maxval:          Maximum value (e.g., missing data)
    :param ignorable:       Keys to be ignored for this discretization
    :return:
    """
    keys = set()
    booleans = dict()
    floats = dict()
    timelist = []
    if timeField is not None:
        timelist.append(timeField)
    for x in t.payloadKeySet():
        if (ignorable is None) or (x not in ignorable):
            type = t.getValueType(x)
            if type == "discrete" or type == "continuous":
                keys.add(x)
    N = len(t)
    for k in keys:
        floats[k] = timelist
    # next(b, None)
    if N>1:
        for i in range(N - 1):
            curr =t[i]
            next =t[i+1]
            for k in keys:
                nv = float(next.getValue(k))
                if math.isnan(nv):
                    nv = 0.0
                cv = float(curr.getValue(k))
                if math.isnan(cv):
                    cv = 0.0
                # if (k.lower() == "total steps"):
                #     print("DEBUG")

                ## Increment
                booleans[k + "_i"] = timelist
                if (nv - cv) >= epsilon:
                    next.setValue(k + "_i", True)
                else:
                    next.setValue(k + "_i", False)

                # ## Decrement (this was the dual of the previous: skipping)
                # booleans[k + "_d"] = timelist
                # if (cv - nv) >= epsilon:
                #     next.setValue(k + "_d", True)
                # else:
                #     next.setValue(k + "_d", False)

                ## Stationariety, Variability (Keeping just stationariety)
                booleans[k + "_s"] = timelist
                # if timeField is not None:
                #     booleans[k + "_vb"] = [k + "_v",timeField]
                # else:
                #     booleans[k + "_vb"] = [k + "_v"]
                if abs(nv - cv) <= epsilon:
                    next.setValue(k + "_s", True)
                    # next.setValue(k + "_vb", False)
                    next.setValue(k + "_v", 0.0)
                else:
                    next.setValue(k + "_s", False)
                    # next.setValue(k + "_vb", True)
                    if abs(cv) <= epsilon:
                        next.setValue(k + "_v", maxval)
                    else:
                        next.setValue(k + "_v", (nv - cv) / cv)

                ## Value Absence
                booleans[k + "_a"] = timelist
                if (abs(nv) <= epsilon):
                    next.setValue(k + "_a", True)
                else:
                    next.setValue(k + "_a", False)
    t.reIndex()
    return booleans, floats
    # return t
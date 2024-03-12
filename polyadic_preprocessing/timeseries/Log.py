import math
import numbers
import random
from copy import deepcopy
from datetime import datetime
from collections import Counter, defaultdict
import ciso8601 as ciso8601
import pandas as pd
from opyenxes.model import XAttributeBoolean, XAttributeLiteral, XAttributeTimestamp, XAttributeDiscrete, XAttributeContinuous
import dateparser
import json
import re
import copy

from timeseries.MultiSet import PolyadicTrace

types = {"literal", "boolean", "discrete", "timestamp", "continuous"}
TRACE_LENGTH = "@trace_len"

def quoted(x):
    return json.dumps(x)

def field(name, val):
    if isinstance(val, str):
        val = json.dumps(val)
    else:
        val = str(val)
    return " var "+quoted(name)+" = "+val+" "

def label(name):
    return name
    #return re.sub('_','',re.sub('\s','', re.sub('\d', 'x', name))).capitalize()

def dict_union(d1 : dict, d2 : dict):
    K = set.union(set(d1.keys()), set(d2.keys()))
    result = {k : set() for k in K}
    for k in K:
        if k in d1 and k in d2:
            result[k] = set.union(d1[k], d2[k])
        elif k in d1:
            result[k] = d1[k]
        else:
            result[k] = d2[k]
    return result

def typecast(type : str, val):
    if type == "literal":
        if val is None:
            return ""
        return str(val)
    elif type == "boolean":
        if val is None:
            return False
        if isinstance(val, str):
            return val == "True"
        elif isinstance(val, bool):
            return val
        return bool(val)
    elif type == "discrete":
        if val is None:
            return 0
        return int(val)
    elif type == "timestamp":
        if val is None or val =="":
            return datetime.MINYEAR
        return ciso8601.parse_datetime(str(val))
    elif type == "continuous":
        if val is None:
            return 0.0
        return float(val)

def get_attribute_type(val):
    if isinstance(val, XAttributeLiteral.XAttributeLiteral) or isinstance(val, str):
        return "literal"
    elif isinstance(val, XAttributeBoolean.XAttributeBoolean) or isinstance(val, bool):
        return "boolean"
    elif isinstance(val, XAttributeDiscrete.XAttributeDiscrete) or isinstance(val, int):
        return "discrete"
    elif isinstance(val, XAttributeTimestamp.XAttributeTimestamp) or isinstance(val, datetime):
        return "timestamp"
    elif isinstance(val, XAttributeContinuous.XAttributeContinuous) or isinstance(val, numbers.Number):
        return "continuous"

def extract_attributes(event, attribs=None):
    if not attribs:
        #attribs = ["concept:name", "lifecycle:transition"]
        attribs = ["concept:name"]
    extracted = {}
    event_attribs = event.get_attributes()
    for att in attribs:
        extracted[att] = event_attribs[att].get_value()
    return extracted


class EventPayload:
    def __init__(self, attributes=None, toExclude=None):
        self.attribute_type = {}
        self.trace_data = {}
        self.size = 0
        if toExclude is None:
            toExclude = {"concept:name", "Label", "lifecycle:transition"}
        self.toExclude = toExclude
        if attributes is None:
            attributes = dict()
        for key, val in attributes:
            self.put(key, val)
        self.keys = set(self.trace_data.keys())

    def __getitem__(self, item):
        return self.trace_data[item]

    def __len__(self):
        return len(self.trace_data)

    def __iter__(self):
        return self.trace_data.__iter__()

    def inInterval(self, key, conv, x):
        return x.begin <= conv(self.trace_data[key]) < x.end

    def getAttributeSet(self):
        return self.trace_data.keys()

    def toJSONObject(self, additionalTraceFields=None):
        if len(self.trace_data)==0:
            return additionalTraceFields
        else:
            if additionalTraceFields is None:
                return copy.deepcopy(self.trace_data)
            else:
                d = dict()
                for key, val in self.trace_data.items():
                    d[key] = val
                for key, val in additionalTraceFields.items():
                    d[key] = val
                return d

    def toHRF(self):
        return " { "+(" ".join(map(lambda x: field(x,self.trace_data[x]), self.trace_data.keys())))+" } "

    def put(self, key, val):
        if (val is not None):
            test = not isinstance(val, numbers.Number)
            if not test:
                test = (not math.isnan(val))
            if test:
                if key not in self.toExclude:
                    if (key == "time:timestamp"):
                        self.putTime(val)
                    else:
                        self.putAny(key, val)
                    return True
        return False

    def putTime(self, val):
        self.size = self.size + 1
        self.attribute_type["time:timestamp"] = "continuous"
        self.trace_data["time:timestamp"] = str(dateparser.parse(str(val)).timestamp())

    def putAny(self, key, val):
        self.size = self.size + 1
        self.attribute_type[key] = get_attribute_type(val)
        self.trace_data[key] = str(val)

    def payloadKeySet(self):
        return self.keys


class Event:
    def __init__(self, activityLabel : str, eventpayload : EventPayload):
        self.eventpayload = eventpayload
        self.activityLabel = activityLabel

    def inInterval(self, key, conv, x):
        if self.eventpayload is None:
            return False
        else:
            return self.eventpayload.inInterval(key,conv, x)

    def toJSONObject(self):
        if self.eventpayload is None:
            return {"__label", self.activityLabel}
        else:
            d = self.eventpayload.toJSONObject()
            d["__label"] = self.activityLabel
            return d

    def toHRF(self):
        S = "\t" + label(self.activityLabel) + " "
        if self.eventpayload is None:
            return  S + " { } "
        else:
            return S + self.eventpayload.toHRF()

    def __repr__(self):
        return self.toHRF()

    def __str__(self):
        return self.toHRF()

    def payloadKeySet(self):
        if self.eventpayload is not None:
            return self.eventpayload.payloadKeySet()
        else:
            return set()

    def datalessEvent(self):
        return Event(self.activityLabel, None)

    def booleanEventExtraction(self, name, payloads=None):
        if payloads is None:
            payloads = set()
        label = name
        pl = dict()
        # if label=="stanek" or label=="rotigotine" or label=="stalevo":
        #     print("S")
        if (not self.hasKey(name)) or (name not in self.eventpayload.attribute_type) or (not typecast("boolean", self.getValue(name))):
            label = "N"+label
        else:
            label = "Y"+label
        for x in payloads:
            if x in self.eventpayload.attribute_type:
                pl[x] = typecast(self.eventpayload.attribute_type[x], self.getValue(x))
        return Event(label, EventPayload(pl.items()))

    def floatEventExtraction(self, name, payloads=None):
        label = name
        if payloads is None:
            payloads = set()
        pl = dict()
        # if label=="stanek" or label=="rotigotine" or label=="stalevo":
        #     print("S")
        if not self.hasKey(name):
            label = "N" + label
        else:
            label = "Y"+label
            pl["payload"] = self.getValue(name)
        for x in payloads:
            if x in self.eventpayload.attribute_type:
                pl[x] = typecast(self.eventpayload.attribute_type[x], self.getValue(x))
        return Event(label, EventPayload(pl.items()))

    def hasPayload(self):
        return self.eventpayload is not None

    def payload(self):
        return self.eventpayload

    def getActivityLabel(self):
        return self.activityLabel

    def getValueType(self, key):
        if self.eventpayload is None:
            return "continuous"
        elif key in self.eventpayload.trace_data:
            return self.eventpayload.attribute_type[key]
        else:
            return "continuous"

    def getValue(self, key):
        if self.eventpayload is None:
            return 0.0
        elif key in self.eventpayload.trace_data:
            return self.eventpayload.trace_data[key]
        else:
            return 0.0

    def setValue(self, key, value):
        if self.eventpayload is None:
            self.eventpayload = EventPayload()
        if self.eventpayload.put(key, value):
            self.eventpayload.attribute_type[key] = get_attribute_type(value)
            self.eventpayload.keys.add(key)

    def __iter__(self):
        if self.eventpayload is not None:
            return self.eventpayload.__iter__()
        else:
            return list()

    def __getitem__(self, key):
        return self.getValue(key)

    def __len__(self):
        if self.eventpayload is None:
            return 0
        else:
            return len(self.eventpayload)

    def hasKey(self, k):
        if self.eventpayload is None:
            return False
        if k not in self.eventpayload.keys:
            return False
        return True


class TracePositional:
    def __init__(self, trace=None, withData=False, withExplicitPayloadMap=None):
        self.declareTypeOf = {}
        self.positional_events = PolyadicTrace()
        self.events = list()
        self.length = 0
        self.keyType = dict()
        self.keys = set()
        self.tracePayload = None
        self.withData = withData
        self.minIndexFor = defaultdict(lambda :-1)
        self.maxIndexFor = defaultdict(lambda :-1)
        if trace is not None:
            if isinstance(trace, list):
                self.trace_name = "None"
                if withExplicitPayloadMap is not None:
                    self.addEventAtTrace("__trace_payload", -1, withExplicitPayloadMap.items())
                for pos, event in enumerate(trace):
                    self.fillTrace(event, pos)
            else:
                trace_attribs = trace.get_attributes()
                if "concept:name" in trace_attribs:
                    self.trace_name = trace_attribs["concept:name"].get_value()
                else:
                    self.trace_name = "None"
                if withData:
                    _payload = trace_attribs.items()
                    event_name = "__trace_payload"
                    self.addEventAtTrace(event_name, -1, _payload)
                for pos, event in enumerate(trace):
                    event_attribs = extract_attributes(event)
                    event_name = event_attribs["concept:name"]
                    _payload = None
                    if withData:
                        _payload = event.get_attributes().items()

                    self.addEventAtTrace(event_name, pos, _payload)
        self.indexing()

    def fillTrace(self, event, pos):
        if isinstance(event, Event):
            self.addEvent(event, pos)
        elif isinstance(event, str):
            self.addEventAtTrace(event, pos)
        elif isinstance(event, tuple):
            event_name, payload = event
            self.addEventAtTrace(event_name, pos, payload)
        elif isinstance(event, list):
            for x in event:
                self.fillTrace(x, pos)

    def subtraceFromInterval(self, key, conv, x):
        L = list(filter(lambda e: e.inInterval(key,conv,x), self.events))
        t = TracePositional(L, self.withData)
        t.setTraceName(self.getTraceName())
        return t

    def toHRF(self):
        if self.tracePayload is None:
            S = " { } : /* " + self.getTraceName() + " */ "+os.linesep
        else:
            S = self.tracePayload.toHF()+" /* "+ self.getTraceName() + " */ "+os.linesep
        l = max(set(self.minIndexFor.keys()).union(set(self.maxIndexFor.keys())))
        #for idx in range(l+1):
        return S+os.linesep.join(map(lambda idx: "["+(" ".join(map(lambda x : x.toHRF(), self.events[self.minIndexFor[idx]:self.maxIndexFor[idx]+1])))+"]", range(l+1))) #os.linesep.join(map(lambda x: x.toHRF(), self.events))

    def toJSONObject(self, additionalTraceFields=None, extendWithEvent=None):
        if additionalTraceFields is None:
            additionalTraceFields = dict()
        traceName = self.getTraceName()
        tpObject = None
        if self.tracePayload is not None:
            tpObject = self.tracePayload.toJSONObject()
        else:
            tpObject = {}
        U = set(self.minIndexFor.keys()).union(set(self.maxIndexFor.keys()))
        if len(U)==0:
            return None
        l = max(U)
        return {
            "__name": self.getTraceName(),
            "__payload": self.tracePayload.toJSONObject(additionalTraceFields) if self.tracePayload is not None else additionalTraceFields,
            "__events": list(map(lambda idx: list(
            (map(lambda x: x.toJSONObject(), self.events[self.minIndexFor[idx]:self.maxIndexFor[idx] + 1]))),
                            range(l + 1)))
        }

    def booleanEventExtraction(self, name, payloads=None,idx=1):
        L = []
        for x in range(idx):
            L.append(Event("Start", None))
        for event in self.events[idx:]:
            L.append(event.booleanEventExtraction(name, payloads))
        t = TracePositional(L, (payloads is not None) and (len(payloads) >0))
        t.setTraceName(self.getTraceName()+"@"+name)
        return t

    def floatEventExtraction(self, name, payloads=None,idx=0):
        L = []
        for x in range(idx):
            L.append(Event("Start", None))
        for event in self.events[idx:]:
            L.append(event.floatEventExtraction(name, payloads))
        t = TracePositional(L, True)
        t.setTraceName(self.getTraceName() + "@" + name)
        return t

    def datalessTrace(self,idx=0):
        L = []
        for x in range(idx):
            L.append("Start")
        for event in self.events[idx:]:
            L.append(event.getActivityLabel())
        t = TracePositional(L, False)
        t.setTraceName(self.getTraceName() + "@label")
        return t

    def copyTraces(self):
        t = TracePositional(self.events, self.withData)
        t.setTraceName(self.getTraceName() + "@label")
        return t

    def EventExtraction(self, booleans, floats, datalessTrace):
        if datalessTrace:
            yield self.datalessTrace()
        else:
            yield self.copyTraces()
        for name, payloads in booleans.items():
            yield self.booleanEventExtraction(name, payloads,1)
        for name, payloads in floats.items():
            yield self.floatEventExtraction(name, payloads,0)



    def getTraceName(self):
        return self.trace_name

    def setTraceName(self, x):
        self.trace_name = x

    def indexing(self):
        for k in self.keys:
            typeInferOf = {type: 0 for type in types}
            for e in self.events:
                if e.getValueType(k) is not None:
                    typeInferOf[e.getValueType(k)] = typeInferOf[e.getValueType(k)] + 1
            self.keyType[k] =  max(typeInferOf, key=typeInferOf.get)

    def payloadKeySet(self):
        if self.keys is None:
            return set()
        else:
            return self.keys

    def __copy__(self):
        cls = self.__class__
        result = cls.__new__(cls)
        result.__dict__.update(self.__dict__)
        return result

    def __deepcopy__(self, memo):
        cls = self.__class__
        result = cls.__new__(cls)
        memo[id(self)] = result
        for k, v in self.__dict__.items():
            setattr(result, k, deepcopy(v, memo))
        return result

    def groupBy(self, key):
        result = dict()
        if self.withData:
            for event in self.events:
                val = event.getValue(key)
                if val not in result:
                    result[val] = list()
                result[val].append(event)
            for event in result.keys():\
                result[event] = TracePositional(result[event], self.withData)
            return result
        else:
            result[0.0] = self
            return result

    def projectWith(self, lstEvents : list[int]):
        this = deepcopy(self)
        eS = set(lstEvents)
        removeK = set()
        for k in this.positional_events:
            kS = set(this.positional_events[k]) - eS
            if len(kS) == 0:
                removeK.add(k)
            else:
                this.positional_events = list(kS)
        for k in removeK:
            this.positional_events.pop(k)

    def __contains__(self, key):
        return self.hasEvent(key)

    def getStringTrace(self):
        l = [None] * self.length
        for label in self.positional_events:
            for i in self.positional_events[label]:
                l[i] = label
        return l

    def getEventsInPositionalTrace(self, label):
        if label not in self.positional_events:
            return list()
        else:
            return self.positional_events[label]

    def hasPos(self, label, pos):
        if label not in self.positional_events:
            return False
        else:
            return pos in self.positional_events[label]

    def __len__(self):
        return len(self.events)

    def __getitem__(self, i):
        return self.events[i]

    def __getattr__(self, item):
        if self.tracePayload is not None:
            return self.tracePayload[item]

    def __iter__(self):
        return self.events.__iter__()

    def hasAttributes(self):
        if self.tracePayload is not None:
            return len(self.tracePayload.getAttributeSet())>0
        else:
            return False

    def getAttributeSet(self):
        if self.tracePayload is not None:
            return self.tracePayload.getAttributeSet()
        else:
            return set()

    def addEvent(self, e:Event, pos=-1):
        label = e.getActivityLabel()
        if pos>=0:
            if pos not in self.minIndexFor:
                self.minIndexFor[pos] = len(self.events)
            self.maxIndexFor[pos] = len(self.events)
            self.length = self.length + 1
            # if label not in self.positional_events:
            #     self.positional_events[label] = list()
            self.positional_events.add(label, pos) #[label].append(pos)
        self.events.append(e)
        if e.hasPayload():
            self.keys = set.union(self.keys, e.payloadKeySet())
            if pos < 0:
                self.tracePayload = e.payload()

    def reIndex(self):
        for e in self.events:
            self.keys = set.union(self.keys, e.payloadKeySet())
        self.indexing()

    def addEventAtTrace(self, label, pos=-1, _payload=None):
        if pos>=0:
            self.length = self.length + 1
            # if label not in self.positional_events:
            #     self.positional_events[label] = list()
            if pos not in self.minIndexFor:
                self.minIndexFor[pos]  = len(self.events)
            self.maxIndexFor[pos] = len(self.events)
            self.positional_events.add(label, pos) #[label].append(pos)
        payload = None
        if _payload is not None:
            payload = EventPayload(_payload)
        e = Event(label, payload)
        self.keys = set.union(self.keys, e.payloadKeySet())
        self.events.append(e)
        if pos<0:
                self.tracePayload = payload

    def addEventAtPositionalTrace(self, label, pos, _payload=None):
        assert (pos >= 0)
        self.addEventAtTrace(label, pos, _payload)


    def hasEvent(self, label):
        if label not in self.positional_events:
            return False
        else:
            return len(self.positional_events[label])>0

    def eventCount(self, label):
        if label not in self.positional_events:
            return 0
        else:
            return len(self.positional_events[label])

    def getValueType(self, k):
        if k not in self.keyType:
            return "continuous"
        else:
            return self.keyType[k]

    def collectDistinctValues(self, withTypeCast : dict[str,str], keys = None):
        if keys is None:
            keys = self.keys
        result = dict()
        for k in keys:
            values = set()
            for idx, e in enumerate(self.events):
                if e.hasKey(k):
                    val = typecast(withTypeCast[k], e.getValue(k))
                    values.add(val)
            result[k] = values
        return result

    def selectEvents(self, ls):
        N = len(self)
        L = list()
        for i in ls:
            if i<N:
                L.append(self.events[i])
        t = TracePositional(L, self.withData)
        t.setTraceName(self.getTraceName())
        return t

    def collectValuesForPayloadEmbedding(self, withTypeCast : dict[str,str],
                                         keys=None,
                                         occurrence = None,
                                         preserveEvents = None,
                                         keepTraceLenth = True,
                                         otherValues=False):
        d = dict()
        if keys is None:
            keys = self.keys
        if preserveEvents is None:
            preserveEvents = set(range(self.length))
        if keepTraceLenth:
            d[TRACE_LENGTH] = self.length
        for k in keys:
            values = list()
            N = 0
            if self.events is not None:
                N = len(self.events)
            for idx, e in enumerate(self.events):
                if idx not in preserveEvents:
                    continue
                if e.hasKey(k):
                    val = typecast(withTypeCast[k], e.getValue(k))
                    d["@"+e.activityLabel+"."+k] = val
                    values.append(val)
                    if otherValues:
                        if idx == 0:
                            d["@first("+k+")"] = val
                        if idx == N:
                            d["@last("+k+")"] = val
            if otherValues and (len(values)>0):
                d["@min("+k+")"] = min(values)
                d["@max("+k+")"] = max(values)
            counter = Counter(values)
            if otherValues and keepTraceLenth:
                if occurrence is None or k not in occurrence:
                    for instance in counter:
                        d["@count("+k+"="+str(instance)+")"] = counter[instance]
                elif k in occurrence:
                    values = set.union(occurrence[k], set(counter.keys()))
                    for value in values:
                        if value in occurrence:
                            d["@count(" + k + "=" + str(value) + ")"] = counter[value]
                        else:
                            d["@count(" + k + "=" + str(value) + ")"] = 0
        return d




class Log:
    def __init__(self, path=None, id=0, withData=False, isTab=False, log=None, isList=False):
        self.path = path
        self.traces = []
        self.max_length = -1
        self.withData = withData
        if (log is None) and (path is not None):
            if isTab and (not isList):
                log = []
                with open(path, 'r') as file1:
                    for t in file1:
                        log.append(t[:-1].split("\t"))
            else:
                with open(self.path) as log_file:
                    log= XUniversalParser().parse(log_file)[id]
        self.unique_events = set()
        self.keys = set()
        self.keyType = dict()
        if log is not None:
            for trace in log:
                self.addTracePositional(trace, withData, isTab)
        self.indexing()


    def deriveHierarchy(self):
        d = defaultdict(set)
        for trace in self.traces:
            for event in trace.events:
                s = event.activityLabel
                LPAR = s.find("(")
                RPAR = s.rfind(")")
                if (LPAR != -1 and RPAR != -1 and LPAR<RPAR):
                    parentEvent = s[s.find("(") + 1:s.rfind(")")]
                    d[parentEvent].add(s)
                else:
                    d[s].add(s)
        return {k:list(v) for k,v in d.items()}

    def subtraceFromInterval(self, key, conv, x):
        L = [y.subtraceFromInterval(key, conv, x) for y in self.traces]
        return Log(withData=self.withData, log=L)

    def toJSONObject(self, additionalTraceFields=None, timeElements=None):
        self.reIndex()
        return {"schema": self.keyType,
                "event_hierarchy":self.deriveHierarchy(),
                "log":list(filter(lambda y: y is not None, map(lambda x: x.toJSONObject(additionalTraceFields, timeElements), self.traces)))
                }

    def toHRF(self):
        return (";"+os.linesep).join(map(lambda x: x.toHRF(), self.traces))

    def selectEvents(self, ls):
        L = list()
        for trace in self.traces:
            L.append(trace.selectEvents(ls))
        return Log(withData=self.withData, log=L)

    def indexing(self):
        for k in self.keys:
            typeInferOf = {type: 0 for type in types}
            value_is_found = False
            for e in self.traces:
                t = e.getValueType(k)
                if t is not None:
                    typeInferOf[t] = typeInferOf[t] + 1
                    value_is_found = True
            if value_is_found:
                self.keyType[k] = max(typeInferOf, key=typeInferOf.get)
            else:
                self.keyType[k] = "continuous"

    def reIndex(self):
        for t in self.traces:
            t.reIndex()
            self.keys = set.union(self.keys, t.payloadKeySet())
        self.indexing()

    def addTracePositional(self, trace, withData=False, isTab=False, withExplicitPayloadMap=None):
        tp = None
        if isinstance(trace, TracePositional):
            tp = trace
        else:
            tp = TracePositional(trace, withData=withData, withExplicitPayloadMap=withExplicitPayloadMap)
        self.keys = set.union(self.keys, tp.payloadKeySet())
        self.max_length = max(self.max_length, tp.length)
        self.traces.append(tp)
        for event in trace:
            if isinstance(event, Event):
                self.unique_events.add(event.getActivityLabel())
            elif isTab:
                if isinstance(event, list):
                    for x in event:
                        self.unique_events.add(x)
                else:
                    self.unique_events.add(event)
            else:
                self.unique_events.add(extract_attributes(event)["concept:name"])

    def getEventSet(self):
        return self.unique_events

    def getTraces(self):
        return self.traces

    def getNTraces(self):
        return len(self.traces)

    def getIthTrace(self, i):
        return self.traces[i]

    def __len__(self):
        return len(self.traces)

    def __getitem__(self, key):
        return self.traces[key]

    def __iter__(self):
        return self.traces.__iter__()

    def EventExtraction(self, b, f, datalessTrace):
        L = []
        for trace in self.traces:
            for x in trace.EventExtraction(b, f, datalessTrace):
                L.append(x)
        return Log(withData=True, log=L)

    def payloadKeySet(self):
        return self.keys

    def getValueType(self, k):
        if k not in self.keyType:
            return None
        else:
            return self.keyType[k]

    def resolvePayload(self, key, value):
        return typecast(self.getValueType(key), value)

    def resolvePayload(self, key : str,  event : Event):
        return self.resolvePayload(key, event.getValue(key))

    def collectDistinctValues(self, ignoreKeys = None):
        if ignoreKeys is None:
            ignoreKeys = set()
        d = {k : set() for k in self.keys if k not in ignoreKeys}
        if ignoreKeys is None:
            for trace in self.traces:
                d = dict_union(d, trace.collectDistinctValues(self.keyType, self.keys))
        else:
            keyType = dict()
            keys = set()
            for k in self.keyType:
                if k not in ignoreKeys:
                    keyType[k] = self.keyType[k]
            for k in self.keys:
                if k not in ignoreKeys:
                    keys.add(k)
            for trace in self.traces:
                d = dict_union(d, trace.collectDistinctValues(keyType, keys))
        return d

    def collectValuesForPayloadEmbedding(self,
                                         distinct_values : dict,
                                         ignoreKeys = None,
                                         preserveEvents=None,
                                         keepTraceLen = True,
                                         setNAToZero = True,
                                         otherValues=False
                                         ):
        if ignoreKeys is None:
            ignoreKeys = set()
        if distinct_values is None or (len(distinct_values) == 0):
            distinct_values = self.collectDistinctValues(ignoreKeys)
        if preserveEvents is None:
            preserveEvents = [list(range(x.length)) for x in self.traces]
        keys = self.keys - ignoreKeys
        traceToEventsToPreserve = zip(self.traces, preserveEvents)
        df = pd.DataFrame(map(lambda x: x[0].collectValuesForPayloadEmbedding(self.keyType, keys, distinct_values, x[1], keepTraceLen, otherValues), traceToEventsToPreserve))
        if setNAToZero:
            df = df.fillna(0)
        return df


from opyenxes.data_in.XUniversalParser import XUniversalParser
from opyenxes.data_out.XesXmlSerializer import XesXmlSerializer
from opyenxes.factory.XFactory import XFactory
import os

def legacy_read_XES_log(path, ithLog = 0):
    with open(path) as log_file:
        return XUniversalParser().parse(log_file)[ithLog]

def legacy_mkdirs(path):
    from pathlib import Path
    Path(path).mkdir(parents=True, exist_ok=True)

def legacy_extractLogCopy(log):
    new_log = XFactory.create_log(log.get_attributes().clone())
    new_log.get_extensions().update(log.get_extensions())
    new_log.__globalTraceAttributes = []
    new_log.__globalTraceAttributes.extend(log.get_global_trace_attributes())
    new_log.__globalEventAttributes = []
    new_log.__globalEventAttributes.extend(log.get_global_event_attributes())
    return new_log

def legacy_split_log(readPath,log_file_tagged,outputPath):
    toRead = os.path.join(readPath, log_file_tagged)
    assert os.path.isfile(toRead)
    log = legacy_read_XES_log(toRead)
    negLog = legacy_extractLogCopy(log)
    posLog = legacy_extractLogCopy(log)
    for trace in log:
        if trace.get_attributes()["Label"].get_value() == "1":
            posLog.append(trace)
        elif trace.get_attributes()["Label"].get_value() == "0":
            negLog.append(trace)
        else:
            assert False
    output_file = os.path.join(outputPath, )
    legacy_mkdirs(output_file)
    with open(os.path.join(output_file, log_file_tagged[:-4]+"_true_true.xes"), "w") as file:
        XesXmlSerializer().serialize(posLog, file)
    with open(os.path.join(output_file, log_file_tagged[:-4]+"_false_false.xes"), "w") as file:
        XesXmlSerializer().serialize(negLog, file)

def legacy_n_traces(log):
    return len(log)

def legacy_split_log(log, posLog, negLog, clazz, clazz_listpos = None, clazz_listneg = None, sample=0.7):
    N = len(log)
    sample = int(sample * len(log))
    i = 0
    for random_trace in legacy_log_shuffle(log):
        if i<sample:
            posLog.append(random_trace)
            if clazz_listpos is not None:
                clazz_listpos.append(clazz)
        else:
            negLog.append(random_trace)
            if clazz_listneg is not None:
                clazz_listneg.append(clazz)
        i = i+1

def legagy_dump_log(posLog, log):
    with open(log, "w") as file:
        XesXmlSerializer().serialize(posLog, file)

def legacy_log_shuffle(log):
    random.shuffle(log)
    return log
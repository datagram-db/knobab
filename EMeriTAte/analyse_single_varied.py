import json
import os.path
from collections import defaultdict

import pandas

def merge(path, N, final):
    # N = 442
    # path = "/home/giacomo/projects/knobab2_loggen/EMeriTAte/osuleaf/"
    L = []
    for i in range(N):
        df = pandas.read_csv(os.path.join(path, str(i)+".csv"))
        df["dataset"] = str(i)
        L.append(df)
    pandas.concat(L).to_csv((final), index=False)



if __name__ == "__main__":
    data = None
    with open("/home/giacomo/projects/knobab2_loggen/EMeriTAte/osuleaf/polyadic_Algo2_dataful.json") as f:
        data = json.load(f)
    data = data["log"]
    N = 6
    time_to_span_to_class_and_listevents = defaultdict(lambda : defaultdict(lambda: defaultdict(list)))
    for trace in data:
        trace_clazz = None
        for event in trace["__events"]:
            event_time = None
            for constituent in event:
                if (constituent["__label"]!="__raw_data"):
                    time_to_span_to_class_and_listevents[int(constituent["time"])][int(constituent["span"])][trace_clazz].append(constituent["__label"])
                else:
                    if trace_clazz is None:
                        trace_clazz = int(constituent["__class"])
                    else:
                        assert trace_clazz == int(constituent["__class"])
                    if event_time is None:
                        event_time = int(constituent["time"])
                    else:
                        assert event_time == int(constituent["time"])
    evens = list(time_to_span_to_class_and_listevents.keys())
    for event in evens:
            for span in list(time_to_span_to_class_and_listevents[event].keys()):
                M = len(time_to_span_to_class_and_listevents[event][span])
                print(f"at {event}:#{span} you have {M} instances of clazzez")
                if M<N:
                    time_to_span_to_class_and_listevents[event].pop(span)
                else:
                    ## OK, we are having enough classes to differentiate for this span and time!
                    ## 1. Payload-based learning within the span and time.
                    ## for then comparing and choosing the span leading to the best overall classification performance
                    d = defaultdict(set)
                    for clazz in time_to_span_to_class_and_listevents[event][span]:
                        for activity in time_to_span_to_class_and_listevents[event][span][clazz]:
                            d[activity].add(clazz)
                    S = {k for k in d if len(d[k])!=M}
                    if len(S)>0:
                        print({k for k in d if len(d[k])!=M})
                    time_to_span_to_class_and_listevents[event][span] = d
            if len(time_to_span_to_class_and_listevents[event])==0:
                time_to_span_to_class_and_listevents.pop(event)
    print(time_to_span_to_class_and_listevents)
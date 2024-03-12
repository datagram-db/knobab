import functools
import numbers
import datetime
import os
from collections import defaultdict
from math import isnan

import loguru
from dateutil import parser
import pandas
from sklearn.metrics import accuracy_score
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier

from timeseries.Bepi import defineIntervals
from timeseries.DrugHalfLifeSimulator import SubstanceOrActiveInData, Chemists
from timeseries.Log import Event, EventPayload, TracePositional, Log
from utils import export_text2, convo, time_in_range


def parse_meds(filename, idCol, timeCol, toRemove=None, day=None, days=None,  label=None, peculiar=None, dump=None, dictionary=None):
    if toRemove is None:
        toRemove = list()
    if days is None:
        days = list()
    if dump is None:
        dump = list()
    data = pandas.read_csv(filename)
    D = dict()
    for index, row in data.iterrows():
        row = dict(row)
        if label is None:
            row["label"] = "Sample"
        else:
            t,k = label
            if peculiar is not None:
                key, value = peculiar
                row[row[key]] = row[value]
            if t:
                row["label"] = row[k]
                row.pop(k, None)
            else:
                row["label"] = k
        row = {k: row[k] for k in row if (not isinstance(row[k], numbers.Number)) or not isnan(row[k])}
        for x in toRemove:
            row.pop(x, None)
        time = parser.parse(row[timeCol])
        TIME = datetime.time(time.hour, time.minute, time.second, time.microsecond)
        row.pop(timeCol, None)
        row.pop(idCol, None)
        if len(days) > 0:
            daysGrouped = {k: set(filter(lambda x: x.endswith(k), row.keys())) for k in days}
            for day, cols in daysGrouped.items():
                d_day = dict()
                for k in cols:
                    kp = k[:-len(day)]
                    d_day[kp] = row[k]
                if day not in D:
                    D[day] = dict()
                if TIME not in D[day]:
                    D[day][TIME] = list()
                if label is None:
                    d_day["label"] = "Sample"
                else:
                    t, k = label
                    if t:
                        d_day["label"] = row[k]
                    else:
                        d_day["label"] = k
                for x in dump:
                    row.pop(x, None)
                D[day][TIME].append(d_day)
        elif day is None:
            DAY = str('{:04d}'.format(time.year))+"-"+str('{:02d}'.format(time.month))+"-"+str('{:02d}'.format(time.day))
            if DAY not in D:
                D[DAY] = dict()
                row.pop(DAY, None)
            if TIME not in D[DAY]:
                D[DAY][TIME] = list()
            for x in dump:
                row.pop(x, None)
            if ("action" not in row) or (row["action"] == "YES"):
                D[DAY][TIME].append(row)
        else:
            DAY = row[day]
            if DAY not in D:
                D[DAY] = dict()
            row.pop(day, None)
            if TIME not in D[DAY]:
                D[DAY][TIME] = list()
            for x in dump:
                row.pop(x, None)
            if ("action" not in row) or (row["action"] == "YES"):
                D[DAY][TIME].append(row)
    if dictionary is not None:
        for day_, time_dict in D.items():
            for time_, ls in time_dict.items():
                for x in ls:
                    if len(x) == 0:
                        continue
                    assert "label" in x
                    oldLabel = x["label"]
                    newLabel = dictionary[oldLabel]
                    value = x[oldLabel]
                    x[newLabel] = value
                    x["label"] = newLabel
                    del x[oldLabel]
    return D


def parseAvversi(filename):
    D = dict()
    data = pandas.read_csv(filename)
    for index, row in data.iterrows():
        row = dict(row)
        TIME = datetime.time(row["hour"], row["minute"], 0, 0)
        if row["day"] not in D:
            D[row["day"]] = dict()
        row.pop("hour", None)
        row.pop("minute", None)
        row["time"] = TIME
        D[row["day"]][TIME] = row
    return D

def correlate_event_with_numerical_vars(timed, id):
    timed.fillna(0,inplace=True)
    # timed['label'] = timed['label'].map({'Amantadine 100mg':1, 'Madopar 50/12.5mg':2,'Madopar CR 100/25mg':3, 'Rotigotine':4,'Stalevo 125/31.5/200mg':5,'Sample':0})
    X = timed.drop(labels=['event','day','time','fulltime'], axis=1)
    y = timed['event']
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2,stratify=y)

    rf = DecisionTreeClassifier(max_depth=5)
    rf.fit(X_train, y_train)
    y_pred = rf.predict(X_test)
    accuracy = accuracy_score(y_test, y_pred)
    print("Accuracy:", accuracy)
    with open("rules_"+id+".txt", "w") as file:
        file.write(os.linesep.join(export_text2(rf, X.columns,show_weights=True)))
        file.write(os.linesep+("Accuracy: ")+str(accuracy))

def plot_time_series(u, id):
    L = list()
    for index, row in u.iterrows():
        row = dict(row)
        row["time"] = convo(row["time"], row["day"]).timestamp()
        L.append(row)
    df = pandas.DataFrame(L).drop(labels=['day'
                                        ], axis=1, inplace=False)
    df.to_csv(id+"_time_series.csv", index=False)


def asTrace(data, name, eventLabel):
    L = list()
    for index, row in data.iterrows():
        row["time"] = row["fulltime"]
        label = "Off"
        if row["event"] == 0:
            label = "Ok"
        # row["__class"] = label
        row = dict(row)
        row.pop("event", None)
        row = {k: row[k] for k in row if (isinstance(k, str)) or (not isnan(k))}
        row["span"] = 1
        L.append(Event(label, EventPayload(row.items())))
    t = TracePositional(L, True)
    t.setTraceName(name)
    return t

baseDate = datetime.date(year=2024,month=1,day=1)
d = datetime.timedelta(minutes=29,seconds=59,microseconds=99999)
dB = datetime.timedelta(minutes=5)


def compute(id, folder, cdn, c):
    loguru.logger.info("Patient: "+id)
    loguru.logger.trace("a. Parsing drug assumptions")
    d1 = parse_meds(os.path.join(folder, 'CIC_PT_' + id + '_MEDS_NEW.csv'), 'userId', 'timestamp', ['action'], label=(True, "medicineName"), peculiar=("medicineName", "medicineDosage"), dump=['medicineDosage','medicineUnit','Notes'], dictionary=c.dToResolve)
    loguru.logger.trace("b. Parsing macro variables")
    d2 = parse_meds(os.path.join(folder, 'macro_CIC_PT_' + id + '.csv'), 'ID', 'Hour', [''], None,
                    [" d1", " d2", " d3", " d4", " d5", " d6", " d7"], label=None)
    loguru.logger.trace("c. Parsing macro full")
    d3 = parse_meds(os.path.join(folder, 'macrofull_CIC_PT_' + id + '.csv'), 'ID', 'Hour', [''], None,
                    [" d1", " d2", " d3", " d4", " d5", " d6", " d7"], label=None)
    loguru.logger.trace("d. Parsing micro variables")
    d4 = parse_meds(os.path.join(folder, 'micro_CIC_PT_' + id + '.csv'), 'ID', 'Hour', ['Unnamed: 0'], "Day", label=None)
    timed = list()
    loguru.logger.trace("e. parsing reactions")
    AVV = parseAvversi(os.path.join(folder, "avversi.csv"))
    dateTimeSet = set()
    origDateToBasic = dict()
    for x, y, z, u in zip(d1, d2, d3, d4):
        origDateToBasic[x] = u
        for t in d1[x]:
            for w in d1[x][t]:
                if len(w) > 1:
                    w['time'] = t
                    w['day'] = u
                    w['fulltime'] = datetime.datetime(baseDate.year, baseDate.month, u, t.hour, t.minute, t.second)
                    dateTimeSet.add(w['fulltime'])
                    found = False
                    if u in AVV:
                        for k in AVV[u]:
                            if time_in_range(k, d, t):
                                w['event'] = AVV[u][k]["label"]
                                found = True
                                break
                    if not found:
                        w['event'] = 'Ok'
                    timed.append(w)
        for t in d2[y]:
            for w in d2[y][t]:
                if len(w) > 1:
                    w['time'] = t
                    w['day'] = u
                    w['fulltime'] = datetime.datetime(baseDate.year, baseDate.month, u, t.hour, t.minute, t.second)
                    dateTimeSet.add(w['fulltime'])
                    found = False
                    if u in AVV:
                        for k in AVV[u]:
                            if time_in_range(k, d, t):
                                w['event'] = AVV[u][k]["label"]
                                found = True
                                break
                    if not found:
                        w['event'] = 'Ok'
                    timed.append(w)
        for t in d3[z]:
            for w in d3[z][t]:
                if len(w) > 1:
                    w['time'] = t
                    w['day'] = u
                    w['fulltime'] = datetime.datetime(baseDate.year, baseDate.month, u, t.hour, t.minute, t.second)
                    dateTimeSet.add(w['fulltime'])
                    found = False
                    if u in AVV:
                        for k in AVV[u]:
                            if time_in_range(k, d, t):
                                w['event'] = AVV[u][k]["label"]
                                found = True
                                break
                    if not found:
                        w['event'] = 'Ok'
                    timed.append(w)
        for t in d4[u]:
            for w in d4[u][t]:
                if len(w) > 1:
                    w['time'] = t
                    w['day'] = u
                    w['fulltime'] = datetime.datetime(baseDate.year, baseDate.month, u, t.hour, t.minute, t.second)
                    dateTimeSet.add(w['fulltime'])
                    found = False
                    if u in AVV:
                        for k in AVV[u]:
                            if time_in_range(k, d, t):
                                w['event'] = AVV[u][k]["label"]
                                found = True
                                break
                    if not found:
                        w['event'] = 'Ok'
                    timed.append(w)
    timed = pandas.DataFrame(timed)
    loguru.logger.trace("f. aggregating events")
    timed['event'] = timed['event'].map({'Ok': 0, 'Off': 1})
    timed.drop(labels=['label'], axis=1,inplace=True)
    timed = timed.groupby(['day', 'time', 'fulltime']).max().reset_index()
    # u = timed[timed.apply(lambda x: x['label'] == 'Sample', axis=1)].groupby(['day', 'time']).max().reset_index()
    # n = timed[timed.apply(lambda x: x['label'] != 'Sample', axis=1)]
    loguru.logger.trace("g. dumping the resulting file")
    plot_time_series(timed, id)
    # timed = pandas.concat([u, n], axis=0)

    loguru.logger.trace("h. aggregating events  by time and day, and dumping")
    timed.sort_values(['day', 'time'], ascending=[True, True
                                                ], inplace=True)
    cols_to_move = ['day', 'time', 'fulltime', 'event']
    timed = timed[cols_to_move + [col for col in timed.columns if col not in cols_to_move]]

    drugAssumptionList = defaultdict(list)
    rs = dict()
    for day, dicts in d1.items():
        # ddd = datetime.date.fromisoformat(day)
        for time, ls in dicts.items():
            for x in ls:
                if "label" in x:
                    drugName = x["label"]
                    cdn.add(drugName)
                    dose = x[drugName]
                    t = datetime.datetime(baseDate.year,baseDate.month,origDateToBasic[day],time.hour,time.minute,time.second)
                    drugAssumptionList[c.resolveName(drugName)].append(SubstanceOrActiveInData(dose, t))
    for k in drugAssumptionList:
        rs[k] = c.retrieve(k).fromAssumptionInTime(drugAssumptionList[k])
    del drugAssumptionList
    l = functools.reduce(lambda a, b: a.add(b, fill_value=0), [pandas.DataFrame(x.inRunningTime(list(timed["fulltime"]))) for x in  rs.values()])
    timed = pandas.concat([timed, l], axis=1)
    timed.to_csv(id + "_full.csv")
    return timed

def getFolder(main_path, id):
    return os.path.join(main_path, id)
    #return '/home/giacomo/Scaricati/AI and Robotics meeting - timeseries analysis - link/analysis/'+id+'/'

def asWeekLog(i):
    L = []
    for k in i:
        x,y = k[0], k[1]
        L.append(asTrace(x,y, "__raw_data"))
    return Log(withData=True, log=L)

def asDailyLog(i):
    L = []
    for k in i:
        x,y = k[0], k[1]
        t = asTrace(x,y, "__raw_data")
        for k,st in t.groupBy('day').items():
            st.setTraceName(str(y)+"_"+str(k))
            L.append(st)
    return Log(withData=True, log=L)

def exploseTimeVariations(L, epsilon, maxval, ign, considerTime):
    D = None
    F = None
    for trace in L:
        b, f = defineIntervals(trace, considerTime, epsilon, maxval, ign)
        if D is None:
            D = b
            F = f
        else:
            for x in b:
                if x not in D:
                    D[x] = set(b[x])
                else:
                    for y in b[x]:
                        D[x] = set(D[x])
                        D[x].add(y)
            for x in f:
                if x not in F:
                    F[x] = set(f[x])
                else:
                    for y in f[x]:
                        F[x] = set(F[x])
                        F[x].add(y)
    L.reIndex()
    return D, F
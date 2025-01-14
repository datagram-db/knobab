import json
import os
import sys
from collections import defaultdict
import os
import re
import pandas


def transform_entry(e, S, minsplit):
    torem = list(set(filter(lambda x : x.endswith("_a") or x.endswith("_s") or x.endswith("_v") or x.endswith("_i"), S)))
    sc = set(e.columns)
    for x in S:
        if x not in sc:
            from sympy.physics.continuum_mechanics.beam import numpy
            e[x] = numpy.nan
    e.drop(torem, axis=1,inplace=True)
    e.fillna(0,inplace=True)
    if minsplit>0:
        for i in range(len(e) - minsplit):
            tmp = e.loc[i:i+minsplit, :]
            # yield pandas.MultiIndex.from_frame(tmp)
            yield {x: pandas.Series(tmp[x].to_numpy().astype(float)) for x in sorted(set(tmp.columns).difference(torem))}
            #numpy.array([pandas.Series(tmp[x].to_numpy().astype(float)) for x in sorted(set(tmp.columns).difference(torem))])
    else:
        # yield pandas.MultiIndex.from_frame(e)
        yield {x: pandas.Series(e[x].to_numpy().astype(float)) for x in sorted(set(e.columns).difference(torem))}

def trasform_user_class(v, S, minsplit):
    for x in v:
        yield from transform_entry(x, S, minsplit)


def transform_users(u, S, minsplit):
    for k, v in u.items():
        yield (list(trasform_user_class(v, S, minsplit)), [k] * len(v))
    # return {k: list(trasform_user_class(v, S, minsplit)) for k, v in u.items()}

def transform_all(L, S, minsplit):
    for u in L:
        yield from transform_users(u, S, minsplit)

def transform(L, S, minsplit):
    yL = []
    xL = []
    for x, y in transform_all(L, S, minsplit):
        for xs, ys in zip(x,y):
            xL.append(xs)
            yL.append(ys)
    import numpy
    xL = pandas.DataFrame(xL)
    yL = numpy.array(yL)
    return xL, yL
    # from sklearn.model_selection import train_test_split
    # X_train, X_test, y_train, y_test = train_test_split(xL, yL, test_size=0.3, stratify=yL)
    # d = dict()
    # p = dict()
    # for name in classifiers:
    #     clf = classifier(name)
    #     try:
    #         clf.fit(X_train, y_train)
    #         y_pred = clf.predict(X_test)
    #         from sklearn.metrics import accuracy_score
    #         accuracy = accuracy_score(y_test, y_pred)
    #         precision = precision_score(y_test, y_pred)
    #     except:
    #         accuracy = 0
    #         precision = 0
    #     print(f"{name}: Accuracy={accuracy}, Precision={precision}")
    #     d[name] = accuracy
    #     p[name] = precision
    # return (d, p)

def json_loader(filename_json, dominsplit=True, toPop=None): #["__label","day","time","fulltime"]
    data = None
    if not os.path.exists(filename_json):
        return None
    with open(filename_json, "r") as f:
        data = json.load(f)
    data = data["log"]
    S = set()
    userL = []
    if toPop is None:
        toPop = []
    minsplit = sys.maxsize
    for user in data:
        prevClass = None
        df = defaultdict(list)
        buildup = []
        for event in user["__events"]:
            payload = event[0]
            # __class = 1 if payload.pop("__class") == "Ok" else 0
            for x in toPop:
                if x in payload:
                    payload.pop(x)
            # payload.pop("__label")
            # payload.pop("day")
            # payload.pop("time")
            # payload.pop("fulltime")
            # payload["class"] = __class
            __class = payload["class"]
            if prevClass == None:
                prevClass = __class
            if prevClass == __class:
                buildup.append(payload)
            else:
                minsplit = min(minsplit, len(buildup))
                df[prevClass].append(pandas.DataFrame(buildup))
                prevClass= __class
            S = S.union(set(payload.keys()))
        minsplit = min(minsplit, len(buildup))
        df[prevClass].append(pandas.DataFrame(buildup))
        userL.append(df)
    if dominsplit is False:
        minsplit = -1
    return transform(userL, S, minsplit)

# def test_over_custom_dataset(custom_dataset_json):
#     return log_json_to_numpy(custom_dataset_json, dominsplit=True)

def csv_loader(folder, dominsplit=True, toPop=None):
    if not os.path.isdir(folder):
        from EMeriTAte.utilities.ConvertSKTimeToCSVFolder import serialize_to_csv_folder
        serialize_to_csv_folder(folder)
    regex = re.compile(r'(\d*)\.csv$')
    S = set()
    if toPop is None:
        toPop = []
    data = []
    for root, dirs, files in os.walk(folder):
        for file in files:
            if regex.match(file):
                data.append((int(regex.match(file).group(1)), file))
    data.sort(key=lambda x: x[0])
    data = [x[1] for x in data]
    userL = []
    minsplit = sys.maxsize
    for user in data:
        df_user_data = pandas.read_csv(os.path.join(folder, user))
        if len(toPop) > 0:
            df_user_data = pandas.concat([df_user_data.pop(x) for x in toPop], axis=1)
        copy = df_user_data.copy()
        copy.pop("class")
        copy.pop("time")
        copy = df_user_data[copy.isna().T.sum() != len(copy.columns)].to_dict('records')
        prevClass = None
        df = defaultdict(list)
        buildup = []
        for payload in copy:
            __class = payload.pop("class")
            payload.pop("time")
            if prevClass == None:
                prevClass = __class
            if prevClass == __class:
                buildup.append(payload)
            else:
                minsplit = min(minsplit, len(buildup))
                df[prevClass].append(pandas.DataFrame(buildup))
                prevClass = __class
            S = S.union(set(payload.keys()))
        minsplit = min(minsplit, len(buildup))
        df[prevClass].append(pandas.DataFrame(buildup))
        userL.append(df)
    if dominsplit is False:
        minsplit = -1
    x, y = transform(userL, S, minsplit)
    return x, y

if __name__ == "__main__":
    folder = "/home/giacomo/projects/knobab2_loggen/EMeriTAte/japanese_vowels/"
    x, y = csv_loader(folder)
    print("OK")

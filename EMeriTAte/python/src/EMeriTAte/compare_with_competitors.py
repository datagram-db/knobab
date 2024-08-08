import json
import os
import sys
from collections import defaultdict

import pandas
from sklearn.metrics import precision_score


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

def classifier(name):
    if name == 'Rocket':
        from sktime.classification.kernel_based import RocketClassifier
        return RocketClassifier(num_kernels=500, use_multivariate="yes")
    elif name == 'TapNet':
        from sktime.classification.deep_learning.tapnet import TapNetClassifier
        return TapNetClassifier(n_epochs=20,batch_size=4)
    elif name == 'EuclideanKNN':
        from sktime.classification.distance_based import KNeighborsTimeSeriesClassifier
        return KNeighborsTimeSeriesClassifier(distance="euclidean")
    elif name == 'CanonicalIntervalForest':
        from sktime.classification.interval_based import CanonicalIntervalForest
        return CanonicalIntervalForest(n_estimators=3, n_intervals=2, att_subsample_size=2)
    elif name == 'ShapeletTransformClassifier':
        from sktime.classification.shapelet_based import ShapeletTransformClassifier
        return ShapeletTransformClassifier()
    elif name == 'SignatureClassifier':
        from sktime.classification.feature_based import SignatureClassifier
        return SignatureClassifier()

classifiers = ['Rocket', 'TapNet', 'EuclideanKNN', 'CanonicalIntervalForest', 'ShapeletTransformClassifier']

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
    from sklearn.model_selection import train_test_split
    X_train, X_test, y_train, y_test = train_test_split(xL, yL, test_size=0.3, stratify=yL)
    d = dict()
    p = dict()
    for name in classifiers:
        clf = classifier(name)
        try:
            clf.fit(X_train, y_train)
            y_pred = clf.predict(X_test)
            from sklearn.metrics import accuracy_score
            accuracy = accuracy_score(y_test, y_pred)
            precision = precision_score(y_test, y_pred)
        except:
            accuracy = 0
            precision = 0
        print(f"{name}: Accuracy={accuracy}, Precision={precision}")
        d[name] = accuracy
        p[name] = precision
    return (d, p)

def log_json_to_numpy(filename_json, dominsplit=False):
    data = None
    if not os.path.exists(filename_json):
        return None
    with open(filename_json, "r") as f:
        data = json.load(f)
    data = data["log"]
    S = set()
    userL = []
    minsplit = sys.maxsize
    for user in data:
        prevClass = None
        df = defaultdict(list)
        buildup = []
        for event in user["__events"]:
            payload = event[0]
            __class = 1 if payload.pop("__class") == "Ok" else 0
            payload.pop("__label")
            payload.pop("day")
            payload.pop("time")
            payload.pop("fulltime")
            payload["class"] = __class
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





if __name__ == "__main__":
    aL = []
    pL = []
    for _ in range(20):
        a, p  = log_json_to_numpy("/home/giacomo/projects/sdd-processing/sdd-processing/log_weekly.json", dominsplit=True)
        aL.append(a)
        pL.append(p)
    pandas.DataFrame(aL).to_csv("accuracy.csv")
    pandas.DataFrame(pL).to_csv("precision.csv")

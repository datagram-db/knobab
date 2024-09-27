import csv
import datetime
import json
import os
import sys
from collections import defaultdict

import numpy
import pandas
from sklearn.metrics import precision_score, accuracy_score, recall_score, f1_score, roc_curve
from sktime.datasets import load_italy_power_demand, load_osuleaf, load_japanese_vowels, load_basic_motions


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
        return RocketClassifier(use_multivariate="yes")
    elif name == 'TapNet':
        from sktime.classification.deep_learning.tapnet import TapNetClassifier
        return TapNetClassifier()
    elif name == 'EuclideanKNN':
        from sktime.classification.distance_based import KNeighborsTimeSeriesClassifier
        return KNeighborsTimeSeriesClassifier(distance="euclidean")
    elif name == 'CanonicalIntervalForest':
        from sktime.classification.interval_based import CanonicalIntervalForest
        return CanonicalIntervalForest()
    elif name == 'ShapeletTransformClassifier':
        from sktime.classification.shapelet_based import ShapeletTransformClassifier
        return ShapeletTransformClassifier()
    elif name == 'SignatureClassifier':
        from sktime.classification.feature_based import SignatureClassifier
        return SignatureClassifier()

classifiers = ['EuclideanKNN', 'Rocket', 'CanonicalIntervalForest', 'ShapeletTransformClassifier', 'TapNet']

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

def test_over_custom_dataset(custom_dataset_json):
    return log_json_to_numpy(custom_dataset_json, dominsplit=True)

def test_over_sktime_dataset(dataset_loader, filename, nclasses=2, ranges=10):
    csv_file = open(f'{filename}.csv', 'w')
    if nclasses>2:
        writer = csv.DictWriter(csv_file, ["name", "accuracy", "macro_precision", "weighted_precision", "macro_recall", "weighted_recall",
         "weighted_recall", "macro_f1", "weighted_f1", "training_ms", "testing_ms"])
    else:
        writer = csv.DictWriter(csv_file, ["name", "accuracy", "precision",  "recall",
                                           "f1", "roc", "training_ms", "testing_ms"])
    from sklearn.model_selection import train_test_split
    xL, yL = dataset_loader()
    X_trainls = []
    X_testls = []
    y_trainls = []
    y_testls = []
    d = {}
    for idx in range(ranges):
        X_train, X_test, y_train, y_test = train_test_split(xL, yL, test_size=0.3, stratify=yL)
        X_trainls.append(X_train)
        X_testls.append(X_test)
        for x in y_train:
            if x not in d:
                n = len(d)
                d[x] = n
        for x in y_test:
            if x not in d:
                n = len(d)
                d[x] = n
        y_train = numpy.array([d[x] for x in y_train])
        y_test = numpy.array([d[x] for x in y_test])
        y_trainls.append(y_train)
        y_testls.append(y_test)
    ls = []
    for name in classifiers:
        for idx in range(ranges):
            clf = classifier(name)
            before_training = datetime.datetime.now()
            clf.fit(X_trainls[idx], y_trainls[idx])
            after_training = datetime.datetime.now()
            y_pred = clf.predict(X_testls[idx])
            after_testing = datetime.datetime.now()
            training_ms = (after_training - before_training).total_seconds()*1000
            testing_ms = (after_testing - after_training).total_seconds()*1000

            if nclasses > 2:
                accuracy = accuracy_score(y_testls[idx], y_pred)
                macro_precision = precision_score(y_testls[idx], y_pred, average='macro')
                macro_recall = recall_score(y_testls[idx], y_pred, average='macro')
                macro_f1 = f1_score(y_testls[idx], y_pred, average='macro')
                weighted_precision = precision_score(y_testls[idx], y_pred, average='weighted')
                weighted_recall = recall_score(y_testls[idx], y_pred, average='weighted')
                weighted_f1 = f1_score(y_testls[idx], y_pred, average='weighted')
                writer.writerow({"name": name,
                     "accuracy": accuracy,
                     "macro_precision": macro_precision,
                     "weighted_precision": weighted_precision,
                     "macro_recall": macro_recall,
                     "weighted_recall": weighted_recall,
                     "macro_f1": macro_f1,
                     "weighted_f1": weighted_f1,
                           "training_ms": training_ms,
                           "testing_ms": testing_ms
                     })
                csv_file.flush()
                print(f"{name} accuracy: {accuracy} with training {training_ms} and testing {testing_ms}")
            else:
                accuracy = accuracy_score(y_testls[idx], y_pred)
                precision = precision_score(y_testls[idx], y_pred)
                recall = recall_score(y_testls[idx], y_pred)
                f1 = f1_score(y_testls[idx], y_pred)
                fpr, tpr, thresholds = roc_curve(y_testls[idx], y_pred)
                writer.writerow({"name": name,
                     "accuracy": accuracy,
                     "precision": precision,
                     "recall": recall,
                     "f1": f1,
                     "roc": {"fpr": fpr, "tpr": tpr, "thresholds": thresholds},
                           "training_ms": training_ms,
                           "testing_ms": testing_ms
                     })
                csv_file.flush()
                print(f"{name} accuracy: {accuracy} with training {training_ms} and testing {testing_ms}")
                if (name=="TapNet") and (idx==1):
                    break
    csv_file.close()

if __name__ == "__main__":
    # test_over_sktime_dataset(lambda :load_italy_power_demand(),"italy_power_demand", nclasses=2)
    # test_over_sktime_dataset(lambda :load_japanese_vowels(),"japanese_vowels", nclasses=6)
    test_over_sktime_dataset(lambda: load_osuleaf(), "osuleaf", nclasses=4)
    # test_over_sktime_dataset(lambda: load_basic_motions(), "load_basic_motions", nclasses=4)

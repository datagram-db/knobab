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

from EMeriTAte.utilities.ERepresentationToSKTime import csv_loader


# from EMeriTAte.utils.ERepresentationToSKTime import csv_loader


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


def test_over_sktime_dataset(dataset_loader, filename, nclasses=2, ranges=10, benchmark_file=None, clazz=None):
    if benchmark_file is None:
        benchmark_file = f'{filename}.csv'
    if clazz is None:
        clazz = classifiers
    print_header = not os.path.exists(benchmark_file)
    print(f"Benchmarking: {filename}. Serializing results in: {benchmark_file}")
    csv_file = open(benchmark_file, 'a')
    if nclasses>2:
        writer = csv.DictWriter(csv_file, ["name", "accuracy", "macro_precision", "weighted_precision", "macro_recall", "weighted_recall",
         "weighted_recall", "macro_f1", "weighted_f1", "training_ms", "testing_ms"])
    else:
        writer = csv.DictWriter(csv_file, ["name", "accuracy", "precision",  "recall",
                                           "f1", "roc", "training_ms", "testing_ms"])
    if print_header:
        writer.writeheader()
    from sklearn.model_selection import train_test_split
    xL, yL = dataset_loader()
    X_trainls = []
    X_testls = []
    y_trainls = []
    y_testls = []
    d = {}
    train_test = []
    split_file = f"{filename}_train_test_split.json" ## For reproducibility
    if not os.path.exists(split_file):
        print("Splitting the dataset anew between training and testing.")
        for idx in range(ranges):
            X_train, X_test, y_train, y_test = train_test_split(xL, yL, test_size=0.3, stratify=yL)
            X_trainls.append(X_train)
            X_testls.append(X_test)
            train_test.append((X_train.index.tolist(), X_test.index.tolist()))
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
        with open(f"{filename}_train_test_split.json", "w") as f:
            json.dump(train_test, f)
    else:
        print("Loading the previously-saved split between training and testing (for reproducibility purposes).")
        with open(f"{filename}_train_test_split.json", "r") as f:
            L = json.load(f)
            assert len(L) == ranges
            for train_test in L:
                train, test = train_test[0], train_test[1]
                X_train, X_test = xL.iloc[train], xL.iloc[test]
                X_trainls.append(X_train)
                X_testls.append(X_test)
                y_train, y_test = yL[train], yL[test]
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
    for name in clazz:
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
    import os

    os.environ['CUDA_VISIBLE_DEVICES'] = "0"

    # test_over_sktime_dataset(lambda :load_italy_power_demand(),"italy_power_demand", nclasses=2, clazz=['EuclideanKNN', 'Rocket', 'CanonicalIntervalForest', 'ShapeletTransformClassifier'])
    test_over_sktime_dataset(lambda: load_osuleaf(), "osuleaf", nclasses=4, clazz=['EuclideanKNN', 'Rocket', 'CanonicalIntervalForest', 'ShapeletTransformClassifier'])
    # test_over_sktime_dataset(lambda: load_basic_motions(), "load_basic_motions", nclasses=4, clazz=['EuclideanKNN', 'Rocket', 'CanonicalIntervalForest', 'ShapeletTransformClassifier'])

    # TODO: this is not considering equal-length time series. Thus, it is deemed important that we are splitting the dataset in here.
    # test_over_sktime_dataset(lambda :csv_loader("japanese_vowels"),"japanese_vowels", nclasses=6, clazz=['TapNet'])
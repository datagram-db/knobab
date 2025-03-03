import os
import re
import datetime
import statistics
from os import listdir
from os.path import isfile, join
import csv
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, roc_curve
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier

from EMeriTAte.computation_steps.learn_from_cpp_csvs import loadDataset
from EMeriTAte.utilities.utils import export_text2

def training(dataset_name, dict_list, first_phase, second_phase, poly, supp, red, nranges=10, nclasses = 2, split=0.3, class_field = "class", criterion="gini", max_depth=5):
    csv_file = f'{dataset_name}'
    assert isinstance(split, float) and split > 0.0
    X = dict_list.drop(labels=[class_field], axis=1)
    y = dict_list[class_field]
    L = None
    fileexists = os.path.isfile(csv_file)
    if nclasses > 2:
        L = ["FirstPhaseAlgo","SecondPhaseAlgo","poly",
         "supp",
         "red",
         "accuracy",
         "macro_precision",
         "weighted_precision",
         "macro_recall",
         "weighted_recall",
         "macro_f1",
         "weighted_f1",
         "model",
             "training_ms",
             "testing_ms"]
    else:
        L = ["FirstPhaseAlgo","SecondPhaseAlgo","poly",
                "supp",
                "red",
                "accuracy",
                "precision",
                "recall",
                "f1",
                "roc",
                "model",
             "training_ms",
             "testing_ms"]

    outcsv = open(csv_file, 'a')
    writer = csv.DictWriter(outcsv, fieldnames=L)
    if not fileexists:
        writer.writeheader()
        outcsv.flush()

    ls_accuracy = []
    ls_precision = []
    ls_recall = []
    ls_f1 = []
    for i in range(nranges):
        X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=split, stratify=y)
        before_training = datetime.datetime.now()
        rf = DecisionTreeClassifier(criterion=criterion, max_depth=max_depth)
        rf.fit(X_train, y_train)
        after_training = datetime.datetime.now()
        before_testing = datetime.datetime.now()
        y_pred = rf.predict(X_test)
        after_testing = datetime.datetime.now()
        training_ms = (after_training - before_training).total_seconds() * 1000
        testing_ms = (after_testing - after_training).total_seconds() * 1000
        if nclasses > 2:
            accuracy = accuracy_score(y_test, y_pred)
            ls_accuracy.append(accuracy)
            macro_precision = precision_score(y_test, y_pred, average='macro')
            ls_precision.append(macro_precision)
            macro_recall = recall_score(y_test, y_pred, average='macro')
            ls_recall.append(macro_recall)
            macro_f1 = f1_score(y_test, y_pred, average='macro')
            ls_f1.append(macro_f1)
            weighted_precision = precision_score(y_test, y_pred, average='weighted')
            weighted_recall = recall_score(y_test, y_pred, average='weighted')
            weighted_f1 = f1_score(y_test, y_pred, average='weighted')
            d = {"FirstPhaseAlgo":first_phase,"SecondPhaseAlgo":second_phase,
                "poly": poly,
                "supp": supp,
                "red": red,
                "accuracy": accuracy,
                "macro_precision": macro_precision,
                "weighted_precision": weighted_precision,
                "macro_recall": macro_recall,
                "weighted_recall": weighted_recall,
                "macro_f1": macro_f1,
                "weighted_f1": weighted_f1,
                "model": os.linesep.join(export_text2(rf, X.columns, show_weights=True)),
                 "training_ms": training_ms,
            "testing_ms":testing_ms
            }
        else:
            accuracy = accuracy_score(y_test, y_pred)
            ls_accuracy.append(accuracy)
            precision = precision_score(y_test, y_pred)
            ls_precision.append(precision)
            recall = recall_score(y_test, y_pred)
            ls_recall.append(recall)
            f1 = f1_score(y_test, y_pred)
            ls_f1.append(f1)
            fpr, tpr, thresholds = roc_curve(y_test, y_pred)
            d = {"FirstPhaseAlgo":first_phase,"SecondPhaseAlgo":second_phase,
                 "poly": poly,
                "supp": supp,
                "red": red,
                "accuracy": accuracy,
                "precision": precision,
                "recall": recall,
                "f1": f1,
                "roc": {"fpr": fpr, "tpr": tpr, "thresholds": thresholds},
                "model": os.linesep.join(export_text2(rf, X.columns, show_weights=True)),
                 "training_ms": training_ms,
            "testing_ms":testing_ms,
            }
        writer.writerow(d)
        d.pop("model")
        print(d)
        outcsv.flush()
    print(f"accuracy: {statistics.fmean(ls_accuracy)} pm {(max(ls_accuracy)-min(ls_accuracy))/2}")
    print(f"precision: {statistics.fmean(ls_precision)} pm {(max(ls_precision)-min(ls_precision))/2}")
    print(f"recall: {statistics.fmean(ls_recall)} pm {(max(ls_recall)-min(ls_recall))/2}")
    print(f"f1: {statistics.fmean(ls_f1)} pm {(max(ls_f1)-min(ls_f1))/2}")

def load_traditional(dataset = "osuleaf", supp = 0):
    # nclasses = 9
    s = 1 if supp == 1.0 else (0 if supp == 0.0 else supp)
    regex = r"output\_csv\_(\d)+\.csv"
    split = 0.3
    class_field = "class"
    criterion = "gini"
    max_depth = 5
    path, elements = f"/home/giacomo/projects/knobab2_loggen/EMeriTAte/{dataset}/polyadic_Algo1_dataless_algo4/poly_s{s}_0", f"/home/giacomo/projects/knobab2_loggen/EMeriTAte/{dataset}/polyadic_Algo1_dataless_algo4/polyadic_Algo1_dataless.json_{s}_0_1_0_clazz={{clazz}}.txt"
    class_files = []
    ## Reading all of the class files that have been dumped
    for f in [f for f in listdir(path) if isfile(join(path, f))]:
        m = re.search(regex, f)
        if m is not None:
            clazz = int(m.group(1))
            # txt = elements.format(clazz=clazz)
            class_files.append((clazz, os.path.join(path, f)))
            # assert os.path.isfile(txt)
    ## Loading all of the datasets belonging to that class
    dict_list = loadDataset(class_files, class_field)
    return dict_list


if __name__ == "__main__":
    # dataset = "load_basic_motions"
    # nclasses = 9
    # supp = 0
    # s = 1 if supp == 1.0 else (0 if supp == 0.0 else supp)
    #
    # regex = r"output\_csv\_(\d)+\.csv"
    # split = 0.3
    # class_field = "class"
    # criterion = "gini"
    # max_depth = 5
    # path, elements = f"/home/giacomo/projects/knobab2_loggen/EMeriTAte/{dataset}/polyadic_Algo1_dataless_algo4/poly_s{s}_0", f"/home/giacomo/projects/knobab2_loggen/EMeriTAte/{dataset}/polyadic_Algo1_dataless_algo4/polyadic_Algo1_dataless.json_{s}_0_1_0_clazz={{clazz}}.txt"
    # class_files = []
    # ## Reading all of the class files that have been dumped
    # for f in [f for f in listdir(path) if isfile(join(path, f))]:
    #     m = re.search(regex, f)
    #     if m is not None:
    #         clazz = int(m.group(1))
    #         txt = elements.format(clazz=clazz)
    #         class_files.append((clazz, os.path.join(path, f)))
    #         assert os.path.isfile(txt)
    # ## Loading all of the datasets belonging to that class
    # dict_list = loadDataset(class_files, class_field)

    # dict_list = load_traditional("osuleaf")
    # training("osuleaf_old2", dict_list, 1, 4, True, 0.0, False, nclasses=6, max_depth=5)
    from EMeriTAte.comsis25.SecondPhase import neu_pipeline
    dict_list = neu_pipeline("/home/giacomo/projects/knobab2_loggen/EMeriTAte/dyskinetic/test") #load_traditional()#
    training("dyskinetic_dataful", dict_list, 1, 4, True, 0.0, False, nclasses=8, max_depth=5)
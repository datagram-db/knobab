import os
import re
from os import listdir
from os.path import isfile, join
import csv
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, roc_curve
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier

from EMeriTAte.computation_steps.learn_from_cpp_csvs import loadDataset
from EMeriTAte.utilities.utils import export_text2

def training(dataset_name, dict_list, first_phase, second_phase, poly, supp, red, nranges=10, lass_field = "class", nclasses = 2):
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
         "model"]
    else:
        L = ["FirstPhaseAlgo","SecondPhaseAlgo","poly",
                "supp",
                "red",
                "accuracy",
                "precision",
                "recall",
                "f1",
                "roc",
                "model"]

    outcsv = open(csv_file, 'w')
    writer = csv.DictWriter(outcsv, fieldnames=L)
    if not fileexists:
        writer.writeheader()
        outcsv.flush()

    for i in range(nranges):
        X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=split, stratify=y)
        rf = DecisionTreeClassifier(criterion=criterion, max_depth=max_depth)
        rf.fit(X_train, y_train)
        y_pred = rf.predict(X_test)
        if nclasses > 2:
            accuracy = accuracy_score(y_test, y_pred)
            macro_precision = precision_score(y_test, y_pred, average='macro')
            macro_recall = recall_score(y_test, y_pred, average='macro')
            macro_f1 = f1_score(y_test, y_pred, average='macro')
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
                "model": os.linesep.join(export_text2(rf, X.columns, show_weights=True))
            }
        else:
            accuracy = accuracy_score(y_test, y_pred)
            precision = precision_score(y_test, y_pred)
            recall = recall_score(y_test, y_pred)
            f1 = f1_score(y_test, y_pred)
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
                "model": os.linesep.join(export_text2(rf, X.columns, show_weights=True))
            }
        print(d)
        writer.writerow(d)
        outcsv.flush()


if __name__ == "__main__":
    path, elements = "/home/giacomo/projects/knobab2_loggen/EMeriTAte/osuleaf/polyadic_Algo1_dataless_algo4/poly_s0_0", "/home/giacomo/projects/knobab2_loggen/EMeriTAte/osuleaf/polyadic_Algo1_dataless_algo4/polyadic_Algo1_dataless.json_0_0_1_0_clazz={clazz}.txt"
    regex = r"output\_csv\_(\d)+\.csv"
    split = 0.3
    class_field = "class"
    criterion = "gini"
    max_depth = 5
    nclasses = 2

    class_files = []
    ## Reading all of the class files that have been dumped
    for f in [f for f in listdir(path) if isfile(join(path, f))]:
        m = re.search(regex, f)
        if m is not None:
            clazz = int(m.group(1))
            txt = elements.format(clazz=clazz)
            class_files.append((clazz, os.path.join(path, f)))
            assert os.path.isfile(txt)
    ## Loading all of the datasets belonging to that class
    dict_list = loadDataset(class_files, class_field)

    training("italy_power_demand", dict_list, 1, 4, True, 0.0, True, nclasses=6)
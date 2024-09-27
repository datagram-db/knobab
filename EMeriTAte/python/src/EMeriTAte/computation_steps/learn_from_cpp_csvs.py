import os
import sys
from collections import defaultdict
from typing import Tuple

import pandas
from sklearn.metrics import accuracy_score, precision_score, recall_score, roc_curve, f1_score
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier

from EMeriTAte.utils import export_text2

from pathlib import Path
# folder = "/home/giacomo/projects/knobab2_loggen/output_model_healthcare/debugged/nopoly_s0_1/"
# class0 = os.path.join(folder, "output_csv_0.csv")
# class1 = os.path.join(folder, "output_csv_1.csv")
# spec = None#"/home/giacomo/projects/knobab2_loggen/output_model_healthcare/debugged/log_weekly.json_0.2_0_0_0_clazz=1.txt"
# modelfile = Path(folder).name + ".txt"


def load_single_file_and_append_class(t:Tuple[int,str])->pandas.DataFrame:
    df0 = pandas.read_csv(t[1], index_col=0, header=None).transpose()
    df0['class'] = t[0]
    return df0

def loadDataset(classls):
    # for idx, x in enumerate(classls):
    #     df0 = pandas.read_csv(x, index_col=0, header=None).transpose()
    #     df0['class'] = idx
    # df0 = pandas.read_csv(class0, index_col=0, header=None).transpose()
    # df0['class'] = 0
    # df1 = pandas.read_csv(class1, index_col=0, header=None).transpose()
    # df1['class'] = 1
    return pandas.concat(map(load_single_file_and_append_class, enumerate(classls)), axis=0, ignore_index=True).fillna(-1)

def readFileForSpec(filename):
    S = set()
    with open(filename, "r") as f:
        for line in f.readlines():
            S.add(line)
            coex = line.find("CoExistence(")
            cho = line.find("Choice(")
            excl = line.find("ExclChoice(")
            firstOpen = line.find('(')
            lastClose = line.rfind(')')
            if (coex==0) or cho==0 or excl==0:
                comma = line.find(',')
                comma2 = line.rfind(',')
                if (comma == comma2):
                    act1 = line[firstOpen + 1:comma].strip()
                    act2 = line[comma + 1:lastClose].strip()
                    if (coex==0):
                        S.add("CoExistence("+act2+","+act1+")")
                    elif (cho==0):
                        S.add("Choice("+act2+","+act1+")")
                    elif (excl==1):
                        S.add("ExclChoice("+act2+","+act1+")")
                else:
                    sys.exit(1)
    return S

class LearnRepresentation:
    def __init__(self, folder, clazz="class", spec=None, criterion="gini", max_depth=5, split=0.3):
        self.clazz = clazz
        self.split = split
        self.max_depth = max_depth
        self.criterion = criterion
        import glob
        class_files = glob.glob(os.path.join(folder, "output_csv_*.csv"))
        class_files = sorted(class_files, key=lambda x: int(x[x.rfind("output_csv_") + len("output_csv_"):x.rfind(".csv")]))
        print(class_files)
        self.nclasses = len(class_files)
        self.spec = spec  # "/home/giacomo/projects/knobab2_loggen/output_model_healthcare/debugged/log_weekly.json_0.2_0_0_0_clazz=1.txt"
        self.modelfile = Path(folder).name + ".txt"
        self.dict_list = loadDataset(class_files)
        self.rf = None
        # if spec is not None:
        #     S = readFileForSpec(spec)
        #     dict_list = self.dict_list[list(set(self.dict_list.columns).intersection(S)) + [self.clazz]]

    def train(self):
        if self.dict_list.empty or (len(set(self.dict_list.columns)) == 1 and (self.clazz in set(self.dict_list.columns))):
            print("No data")
        else:
            X_train = X = self.dict_list.drop(labels=[self.clazz], axis=1)
            y_train = y = self.dict_list[self.clazz]
            if self.split > 0.0:
                X_train, _, y_train, _ = train_test_split(X, y, test_size=self.split, stratify=y)
            self.rf = DecisionTreeClassifier(criterion=self.criterion, max_depth=self.max_depth)
            self.rf.fit(X_train, y_train)


    def test(self, poly=False, supp=0.0, red=False):
        if self.dict_list.empty or (len(set(self.dict_list.columns)) == 1 and ("class" in set(self.dict_list.columns))):
            print("No data: cannot run the prediction!")
            # return dict()
        else:
            X = self.dict_list.drop(labels=['class'], axis=1)
            y = self.dict_list['class']
            X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, stratify=y)
            rf = DecisionTreeClassifier(criterion="gini", max_depth=5)
            rf.fit(X_train, y_train)
            y_pred = rf.predict(X_test)


            if self.nclasses > 2:
                accuracy = accuracy_score(y_test, y_pred)
                macro_precision = precision_score(y_test, y_pred, average='macro')
                macro_recall = recall_score(y_test, y_pred, average='macro')
                macro_f1 = f1_score(y_test, y_pred, average='macro')
                weighted_precision = precision_score(y_test, y_pred, average='weighted')
                weighted_recall = recall_score(y_test, y_pred, average='weighted')
                weighted_f1 = f1_score(y_test, y_pred, average='weighted')
                d = {"poly": poly,
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
                d = {"poly": poly,
                     "supp": supp,
                     "red": red,
                     "accuracy": accuracy,
                     "precision": precision,
                     "recall": recall,
                     "f1": f1,
                     "roc": {"fpr": fpr, "tpr": tpr, "thresholds": thresholds},
                     "model": os.linesep.join(export_text2(rf, X.columns, show_weights=True))
                     }


            # print(d)
            return d
            # print("Accuracy:", accuracy)
            # with open(modelfile, "w") as file:
            #     file.write(os.linesep.join(export_text2(rf, X.columns, show_weights=True)))
            #     file.write(os.linesep + ("Accuracy: ") + str(accuracy))

def genfolder(poly, s, red):
    poly = "poly" if (poly or (int(poly) == 1)) else "nopoly"
    red = 1 if (red or (int(red) == 1)) else 0
    return f"{poly}_s{s}_{red}"


if __name__ == "__main__":
    outcomes = []
    path = "/home/giacomo/projects/knobab2_loggen/output_model_healthcare/debugged/"
    # name = genfolder(1, 1.0, 1)
    # abs_folder = os.path.join(path, name)
    # lr = LearnRepresentation(abs_folder)
    # for _ in range(20):
        # lr.test(1, 1.0, 1, outcomes)
    for name in os.listdir(path):
        abs_folder = os.path.join(path, name)
        if os.path.isdir(abs_folder):
            print(name)
            lr = LearnRepresentation(abs_folder)
            arr = name.split("_")
            arr[0] = 0 if arr[0] == "nopoly" else 1
            arr[1] = float(arr[1][1:])
            for _ in range(20):
                x = lr.test(arr[0], arr[1], int(arr[2])==1)
                if x is not None:
                    outcomes.append(x)

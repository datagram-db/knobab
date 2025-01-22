import copy
import math
from collections import defaultdict

import matplotlib.ticker as mticker
import pandas
from functools import reduce
from tabulate import tabulate
tabulate.LATEX_ESCAPE_RULES = {}

def latex_format_cell(val):
    avg_expe = math.floor(math.log10(abs(val)))
    val = round(val / 10 ** avg_expe, 2)
    return f"{val:.2f}\\cdot 10^{{{avg_expe}}}"

# define a Custom aggregation
# function for finding total
def pm(series):
      return (series.max() - series.min())/2

from dataclasses import dataclass

@dataclass
class FieldToPlot:
    avg: float = 0
    pm: float = 0
    std: float = 0

    def __str__(self):
        avg_expe = math.floor(math.log10(abs(self.avg)))
        pm_to_avg_exp = round(self.pm / 10 ** avg_expe, 2)
        avg_mant = round(self.avg / 10 ** avg_expe, 2)
        to_print_pm = "0.00" if self.pm == 0 else ("\\varepsilon" if pm_to_avg_exp == 0.0 else pm_to_avg_exp)
        return f"$({avg_mant} \\pm {to_print_pm})\\cdot 10^{{{avg_expe}}}$" #;\;\; \mathit{{{latex_format_cell(self.std)}}}

    def as_percentage_string(self):
        avg_mant = self.avg * 100.0
        avg_mant = round(avg_mant, 2)
        pm = self.pm * 100.0
        pm = round(pm, 2)
        to_print_pm = "0.00" if self.pm == 0.0 else ("\\varepsilon" if self.pm < 0.009 else pm)
        return f"${avg_mant} \\pm {to_print_pm}$"


    def __repr__(self):
        return self.__str__()


def plot_accuracy_table(file, columns_of_interest, rewriting_names, drop=None):
    assert len(columns_of_interest) == len(rewriting_names)
    data = pandas.read_csv(file)
    if drop is not None and len(drop)>0:
        data.drop(columns=drop, axis=1, inplace=True)
    # columns = set(data.columns) - {"name"}
    gb = data.groupby('name')
    # dataset_to_results = {}
    gb = gb.agg({('avg', 'mean'), ('pm', pm), ('std', 'std')})
    names = list(gb.index)
    headers = None
    table = []
    for name, dct in zip(names, gb.to_dict("records")) :
        vals = defaultdict(FieldToPlot)
        for (col, field), val in dct.items():
            if col in columns_of_interest:
                setattr(vals[col], field, val)
        if headers is None:
            headers = ["Algorithm"] + rewriting_names
        table.append([name] + [(vals[x]).as_percentage_string() for x in columns_of_interest])
    print(tabulate(table, headers, tablefmt="latex_raw"))

def aggregate_running_times_tables(dataset_to_file_names, names_column, columns_of_interest, rewriting_names, **kwargs):
    L = []
    for dataset_name, dataset_path in dataset_to_file_names.items():
        df = pandas.read_csv(dataset_path)
        df = df[[names_column] + columns_of_interest]
        gb = df.groupby(names_column)
        gb = gb.agg('mean')
        names = list(gb.index)
        for name, dct in zip(names, gb.to_dict("records")):
            d = {}
            d["Algorithm"] = kwargs.get(name, name)
            d["Dataset"] = dataset_name
            for k_org, k_dst in zip(columns_of_interest, rewriting_names):
                val = dct.pop(k_org)
                d = copy.deepcopy(d)
                d["Phase"] = k_dst
                d["Time (ms)"] = val
                L.append(d)
    pandas.DataFrame(L).to_csv("../../../../running_times.csv", index=False)

if __name__ == "__main__":
    # plot_accuracy_table("/home/giacomo/projects/knobab2_loggen/EMeriTAte/italy_power_demand.csv",
    #                     ['accuracy', 'precision', 'recall', 'f1'],
    #                     ['Accuracy (\%)', 'Precision (\%)', 'Recall (\%)', 'F1 (\%)'],
    #                     ["roc"])
    #
    plot_accuracy_table("/home/giacomo/projects/knobab2_loggen/EMeriTAte/japanese_vowels.csv",
                        ['accuracy', 'macro_precision', 'macro_recall', 'macro_f1'],
                        ['Accuracy (\%)', 'Macro Precision (\%)', 'Macro Recall (\%)', 'Macro F1 (\%)'])


    # plot_accuracy_table("/home/giacomo/projects/knobab2_loggen/EMeriTAte/load_basic_motions.csv",
    #                     ['accuracy', 'macro_precision', 'macro_recall', 'macro_f1'],
    #                     ['Accuracy (\%)', 'Macro Precision (\%)', 'Macro Recall (\%)', 'Macro F1 (\%)'])

    plot_accuracy_table("/home/giacomo/projects/knobab2_loggen/EMeriTAte/osuleaf.csv",
                        ['accuracy', 'macro_precision', 'macro_recall', 'macro_f1'],
                        ['Accuracy (\%)', 'Macro Precision (\%)', 'Macro Recall (\%)', 'Macro F1 (\%)'])

    # plot_accuracy_table("/home/giacomo/projects/knobab2_loggen/EMeriTAte/dyskinetic.csv",
    #                     ['accuracy', 'precision', 'recall', 'f1'],
    #                     ['Accuracy (\%)', 'Precision (\%)', 'Recall (\%)', 'F1 (\%)'],
    #                     ["roc"])

    # plot_accuracy_table("/home/giacomo/projects/knobab2_loggen/EMeriTAte/dyskinetic.csv",
    #                     ['accuracy', 'macro_precision', 'macro_recall', 'macro_f1'],
    #                     ['Accuracy (\%)', 'Macro Precision (\%)', 'Macro Recall (\%)', 'Macro F1 (\%)'])

    aggregate_running_times_tables({"Italy Power Demand": "/home/giacomo/projects/knobab2_loggen/EMeriTAte/italy_power_demand.csv",
                                    "Japanese Vowels": "/home/giacomo/projects/knobab2_loggen/EMeriTAte/japanese_vowels.csv",
                                    "OsuLeaf": "/home/giacomo/projects/knobab2_loggen/EMeriTAte/osuleaf.csv",
                                    "Basic Motions": "/home/giacomo/projects/knobab2_loggen/EMeriTAte/load_basic_motions.csv",
                                    "Dyskinetic Events": "/home/giacomo/projects/knobab2_loggen/EMeriTAte/dyskinetic.csv"},
                                   "name",
                                   ["training_ms", "testing_ms"], ["Training", "Testing"], ShapeletTransformClassifier="STC", CanonicalIntervalForest="CIF", EuclideanKNN="E-KNN")
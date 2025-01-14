import math
from collections import defaultdict

import matplotlib.ticker as mticker
import pandas
from functools import reduce



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
        return f"$({avg_mant} \\pm {to_print_pm})\\cdot 10^{{{avg_expe}}};\;\; \mathit{{{latex_format_cell(self.std)}}}$"

    def __repr__(self):
        return self.__str__()


if __name__ == "__main__":
    print(latex_format_cell(0.0000002567))
    data = pandas.read_csv("/home/giacomo/projects/knobab2_loggen/EMeriTAte/japanese_vowels.csv")
    columns = set(data.columns) - {"name"}
    gb = data.groupby('name')
    dataset_to_results = {}
    for dct in  gb.agg({('avg', 'mean'), ('pm', pm), ('std', 'std')}).to_dict("records"):
        vals = defaultdict(FieldToPlot)
        for (col, field), val in dct.items():
            setattr(vals[col], field, val)
        print(vals)
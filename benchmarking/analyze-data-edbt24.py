#! /usr/bin/env python3
__author__ = "Giacomo Bergami"
__copyright__ = "Copyright 2023, KnoBAB"
__credits__ = ["Giacomo Bergami"]
__license__ = "GPL"
__version__ = "3.0"
__maintainer__ = "Giacomo Bergami"
__email__ = "bergamigiacomo@gmail.com"
__status__ = "Production"

import pandas
def print_hi(name):
    # Use a breakpoint in the code line below to debug your script.
    print(f'Hi, {name}')  # Press Ctrl+F8 to toggle the breakpoint.

def get_dataset_and_(d):
    fn:str = d['log_filename']
    assert fn.endswith(".tab")
    N = fn[:-4].split("_")[-1].split("/")[-1]
    if d["queries_plan"] == "nfmcp23":
        d["queries_plan"] = "mdpi23"
    if N == "full" or (N == "all"):
        d['log_size'] = int(d['n_traces'])
    else:
        d['log_size'] = int(N)
    if "Hospital" in fn:
        d['dataset'] = "Hospital"
    elif "FoodBroker" in fn:
        d['dataset'] = "FoodBroker"
    elif "Cybersecurity" in fn:
        d['dataset'] = "Cybersecurity"
    else:
        d['dataset'] = "NA"
    mf = d['model_filename']
    if (mf is not None) and (not pandas.isna(mf)):
        if "alt_response" in mf:
            d['model_filename'] = "AltResponse"
        elif "alt_precedence" in mf:
            d['model_filename'] = "AltPrecedence"
        elif "chain_response" in mf:
            d['model_filename'] = "ChainResponse"
        elif "chain_precedence" in mf:
            d['model_filename'] = "ChainPrecedence"
        return d
    else:
        return dict()


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    f = pandas.read_csv("/home/giacomo/Scaricati/edbt24/benchmarks/benchmark_wcs2.csv")
    l = []
    for d in map(lambda x : dict(x[1]), f.iterrows()):
        d = get_dataset_and_(d)
        if (len(d)>0):
            l.append(d)
    f = pandas.DataFrame(l)[['dataset', 'log_size', 'model_ltlf_query_time', 'model_size', 'queries_plan', 'model_filename', 'n_acts', 'n_traces']]
    f.to_csv("benchmark2_wcs.csv", index=False)
    print("f")

# See PyCharm help at https://www.jetbrains.com/help/pycharm/

import math
import os.path
from itertools import repeat
from pathlib import Path
import pandas
from sktime.datasets import load_italy_power_demand, load_osuleaf, load_japanese_vowels, load_basic_motions

from EMeriTAte import EMeriTAte

_datasets = {"italy_power_demand": load_italy_power_demand,
     "osuleaf": load_osuleaf,
     "japanese_vowels": load_japanese_vowels,
     "load_basic_motions": load_basic_motions}

def _convert_format_to_csv_folder(ipd, clazz_name, timedim_name, foldername, index_acc=0):
    Path(foldername).mkdir(parents=True, exist_ok=True)
    classes_converter = dict()
    data = ipd[0]
    classes = ipd[1]
    dimensions = list(data.columns)
    if clazz_name in dimensions:
        raise RuntimeError(f"ERROR: the class dimension {clazz_name} cannot be a dimension name within the dataset!")
    if timedim_name in dimensions:
        raise RuntimeError(f"ERROR: the timestamp column {timedim_name} cannot be a dimension name within the dataset!")
    timestamps = set()
    environments = set()
    parsed_as_sets = []
    # data = data.reset_index()
    for index, row in data.iterrows():
        as_dict = {x: dict(row[x]) for x in dimensions}
        parsed_as_sets.append((index, as_dict))
        timestamps = set.union(*map(lambda x: set(x.keys()), as_dict.values())).union(timestamps)
        environments.add(index)

    timestamps = list(timestamps)
    timestamps.sort()

    for index, row in parsed_as_sets:
        clazz = classes[index]
        if not clazz in classes_converter:
            n = len(classes_converter)
            classes_converter[clazz] = n
            clazz = n
        else:
            clazz = classes_converter[clazz]
        row[clazz_name] = dict(zip(timestamps, repeat(clazz)))
        row[timedim_name] = dict(zip(timestamps, timestamps))
        row_parsed = {dim: [dictionary[t] if t in dictionary else math.nan for t in timestamps] for dim, dictionary in row.items()}
        local_pd = pandas.DataFrame(row_parsed)
        local_pd.to_csv(os.path.join(foldername, f"{index+index_acc}.csv"),index=False)
    return max(environments)+index_acc

def serialize_to_csv_folder(path, loader=None, train=None, test=None):
    if loader is None:
        assert path in _datasets
        loader = _datasets[path]
    if train is None:
        train="TRAIN"
    if test is None:
        test="TEST"
    ipd_tr = loader(train)
    idx = _convert_format_to_csv_folder(ipd_tr, "class", "time", path)
    ipd_te = loader(test)
    idx2 = _convert_format_to_csv_folder(ipd_te, "class", "time", path, idx + 1)
    print(f"Train: {idx}" + os.linesep)
    print(f"Test: {idx2 - idx}" + os.linesep)
    print(f"Total: {idx2}")
    print(f"{path} successfully serialised!")

import math
import os.path
from itertools import repeat
from pathlib import Path
import pandas
from sktime.datasets import load_italy_power_demand, load_osuleaf, load_japanese_vowels, load_basic_motions

from EMeriTAte import EMeriTAte


def convert_format_to_csv_folder(ipd, clazz_name, timedim_name, foldername, index_acc=0):
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


def extract_dataset_for_EMeriTAte(path, loader, train=None, test=None):
    if train is None:
        train="TRAIN"
    if test is None:
        test="TEST"
    ipd_tr = loader(train)
    idx = convert_format_to_csv_folder(ipd_tr, "class", "time", path)
    ipd_te = loader(test)
    idx2 = convert_format_to_csv_folder(ipd_te, "class", "time", path, idx + 1)
    with open(f"{path}.train_test_split.txt", "w") as f:
        f.write(f"Train: {idx}" + os.linesep)
        f.write(f"Test: {idx2 - idx}" + os.linesep)
        f.write(f"Total: {idx2}")
    print(f"{path} successfully serialised!")

from loguru import logger
import sys

if __name__ == "__main__":
    # path = "italy_power_demand"
    # loader = load_italy_power_demand
    # extract_dataset_for_EMeriTAte(path, loader)
    logger.remove(0)
    f = sys.stdout
    logger.add(f, level="TRACE")
    # replace = {"fulltime"}


    # path = "osuleaf"
    # loader = load_osuleaf
    # extract_dataset_for_EMeriTAte(path, loader)
    #
    # path = "japanese_vowels"
    # loader = load_japanese_vowels
    # extract_dataset_for_EMeriTAte(path, loader)
    #
    # path = "load_basic_motions"
    # loader = load_basic_motions
    # extract_dataset_for_EMeriTAte(path, loader)
    conversion = ["0", "1"]
    e = EMeriTAte("trace",  # Collective name for the environment types
                  # Folder containing only .csv files, which file name is the environment
                  #  name
                 "/home/giacomo/projects/knobab2_loggen/EMeriTAte/italy_power_demand", #"/home/giacomo/projects/polyadic_processing/raw_data",
                  # CSV column containing the class information [0,1]
                  "class",
                  # CSV column containing the timestamp information
                  "time",
                  # \epsilon parameter
                  0.00000001,
                  # Arbitrary large number
                  10000000000000.0,
                  # Fields not to be considered for the mining
                  {'amantadine', 'day', 'madopar 50', 'madopar CR', 'rotigotine', 'span', 'stalevo', 'stanek'},
                  None,
                  # Converts the number from the CSV classes into labels
                  conversion,
                  None,
                  # Fields to be ignored while loading the data
                  ["day", "span", "__class", "__label", "fulltime"],
                  # Support mining
                  0.0)
    e.run()
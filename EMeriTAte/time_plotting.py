import json
import math
from statistics import mean

import pandas

ls = [
{"name": "Italy Power Demand",
 "file": "benchmark_italy.json",
 "as": "italy_power_demand"},
{"name": "OsuLeaf",
 "file": "benchmark_osuleaf.json",
 "as": "osuleaf"},
{"name": "Basic Motions",
 "file": "benchmark_load_basic_motions.json",
 "as": "load_basic_motions"},
    {"name": "Dyskinetic Events",
     "file": "benchmark_dyskinetic.json",
     "as": "dyskinetic"}
]

def mining():
    results = []
    for x in ls:
        field = x["name"]
        data = None
        total_time_mining = []
        loading = []
        total_serial = []
        with open(x["file"]) as f:
            dat = json.load(f)
            for y in dat:
                total_time_mining.append(y["total_time_mining"])
                loading.append(y["loading"])
                total_serial.append(y["total_event_time_serial"] + y["serial"])

        results.append({"name": x["name"],
                        "algorithm": "EMeriTAte",
                        "Mining": mean(total_time_mining),
                        "Loading": mean(loading),
                        "Serialization": mean(total_serial)})

    old_algo = pandas.DataFrame(results)

    pandas.concat([old_algo, pandas.read_csv("emeritatedf_times_new.csv")], axis=0, ignore_index=True).to_csv(
        "total_emeritate_times.csv", index=False)


ls2 = [
{"name": "Italy Power Demand",
 "file": "python/src/EMeriTAte/comsis25/italy_power_demand",
 "file2": "python/src/EMeriTAte/comsis25/italy_power_demand_old"},
{"name": "OsuLeaf",
 "file": "python/src/EMeriTAte/comsis25/osuleaf",
 "file2": "python/src/EMeriTAte/comsis25/osuleaf_old"},
{"name": "Basic Motions",
 "file": "python/src/EMeriTAte/comsis25/load_basic_motions",
 "file2": "python/src/EMeriTAte/comsis25/load_basic_motions_old"}#,
 #    {"name": "Dyskinetic Events",
 # "file": "python/src/EMeriTAte/comsis25/italy_power_demand",
 # "file2": "python/src/EMeriTAte/comsis25/italy_power_demand_old"}
]

def training():
    results = []
    for x in ls2:
        df = pandas.read_csv(x["file"])
        training = df.loc[:, 'training_ms'].mean()
        df_old = pandas.read_csv(x["file2"])
        training_old = df_old.loc[:, 'training_ms'].mean()
        results.append({"Dataset": x["name"],
         "algorithm": "EMeriTAte",
         "Phase": "Training",
         "Time (ms)": training_old})
        results.append({"Dataset": x["name"],
         "algorithm": "EMeriTAte+DF",
         "Phase": "Training",
         "Time (ms)": training})
    pandas.DataFrame(results).to_csv(
        "total_emeritate_training_times.csv", index=False)

if __name__ == "__main__":
    with open("/home/giacomo/projects/knobab2_loggen/benchmarks_dtfinal.json", "r")as f:
        data = json.load(f)
    for record in data:
        dataset = record["folder"].split("/")[-1]
        print(dataset)
        uel = []
        for y in record["all_stats"]:
            uel.append(y["unique_event_label"])
        print(f"{dataset}: {max(uel)}")
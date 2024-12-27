import pandas
import json
import itertools

if __name__ == "__main__":
    filename = "/home/giacomo/projects/DTMining/benchmarks.json"
    with open(filename, "r") as f:
        j = json.load(f)
    j = pandas.json_normalize(j)

    stats = j.pop("all_stats")
    stats = pandas.concat(map(pandas.json_normalize,stats),axis=0)

    stats_vector = stats.pop("stats_vector")
    stats_vector = pandas.concat(map(pandas.json_normalize, stats_vector), axis=0)

    fine_stats = stats_vector.pop("fineStats")
    fine_stats = pandas.concat(map(pandas.json_normalize, fine_stats), axis=0)
    j.to_csv(filename+"_experiments.tab",index=False,sep="\t")
    stats.to_csv(filename+"_tracewise.tab",index=False,sep="\t")
    stats_vector.to_csv(filename+"_segmentwise.tab",index=False,sep="\t")
    fine_stats.to_csv(filename+"_timestampwise.tab",index=False,sep="\t")
import glob
import os.path
from pathlib import Path
import pandas
import math

supps = set()
reds = set()
poly = set()
rec = set()
filename_fileds = ["mining_supp","reduction","isFilenamePolyadic","reclassify"]
S = set(["Choice", "RespExistence", "Response", "ChainResponse", "Precedence", "ChainPrecedence", "CoExistence", "Succession", "ChainSuccession", "Init", "End", "Exists", "Absence", "Choice", "ExclChoice"])
root_dir = "/home/giacomo/projects/knobab2_loggen/output_model_healthcare/"
untimed = set(["Choice","RespExistence","CoExistence","Choice", "ExclChoice"])
timed = S.difference(untimed)
# desirGlobal = dict()

# root_dir needs a trailing slash (i.e. /root/dir/)
L = []
L2 = []
for filename in glob.iglob(root_dir + '**/*.txt', recursive=True):
    with open(filename, 'r') as f:
        stem = Path(filename).stem.split("_")
        clazz = stem[-1].replace("clazz=", "")
        stem = stem[:-1]
        d = dict(zip(filename_fileds, stem[-len(filename_fileds):]))
        dtmp = dict()
        d["class"] = clazz
        d["filename"] = "_".join(stem[:-len(filename_fileds)])
        key = tuple(stem[-len(filename_fileds):])
        count = 0
        for line in f:
            count+=1
            for clauseName in S:
                if line.startswith(clauseName):
                    if clauseName not in dtmp:
                        dtmp[clauseName] = 1
                    else:
                        dtmp[clauseName] += 1
        d["Total"] = count
        my_copy2 = {key: value for key, value in dtmp.items()}
        for clauseName in S:
            if clauseName in dtmp:
                dtmp[clauseName] = float(dtmp[clauseName])#/float(count)
                my_copy2[clauseName] = float(my_copy2[clauseName])/float(count)
        my_copy = {key: value for key, value in d.items()}
        d["timed"] = float(sum(map(lambda x: dtmp.get(x, 0.0), timed)))
        d["untimed"] = float(sum(map(lambda x: dtmp.get(x, 0.0), untimed)))
        my_copy["timed"] = float(sum(map(lambda x: my_copy2.get(x, 0.0), timed)))
        my_copy["untimed"] = float(sum(map(lambda x: my_copy2.get(x, 0.0), untimed)))
        # desirability = 0.0
        # if count>0:
        #     desirability = float(sum(map(lambda x: d.get(x, 0.0), timed)))/float((1+ sum(map(lambda x: d.get(x, 0.0), untimed))) * (count))
        # desirGlobal[key] = desirability * desirGlobal.get(key, 1.0)
        # d["desirability"] = desirability
        L.append(d)
        L2.append(my_copy)

H = ["filename","class","timed", "untimed"]+filename_fileds
df = pandas.DataFrame(L).fillna(0)
df = df[ H + [ col for col in df.columns if col not in H ] ]
df.sort_values(by=filename_fileds, ascending=[True for _ in range(len(filename_fileds))]).to_csv(os.path.join(root_dir, "results_tot.csv"),index=False)
df = pandas.DataFrame(L2).fillna(0)
df = df[ H + [ col for col in df.columns if col not in H ] ]
df.sort_values(by=filename_fileds, ascending=[True for _ in range(len(filename_fileds))]).to_csv(os.path.join(root_dir, "results.csv"),index=False)

# L = []
# for tup,val in desirGlobal.items():
#     d = dict(zip(filename_fileds, list(tup)))
#     d["desirability"] = val
#     L.append(d)
# df = pandas.DataFrame(L).fillna(0).sort_values(by="desirability", ascending=[True]).to_csv(os.path.join(root_dir, "desire.csv"),index=False)

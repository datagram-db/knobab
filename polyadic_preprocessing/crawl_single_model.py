import os.path
import sys

file = "/home/giacomo/projects/knobab2_loggen/output_model_healthcare/removed_non_rilevazione/log_weekly.json_1_1_1_0_clazz=1.txt"

d_exists = dict()
d_absences = dict()
acts = set()

with open(file, "r") as f:
    for line in f.read().splitlines():
        if line.startswith("Exists"):
            firstOpen = line.find('(')
            lastClose = line.rfind(')')
            lastPar = line.rfind('§')
            act = line[firstOpen + 1:lastPar - 1].strip()
            acts.add(act)
            if act not in d_exists:
                d_exists[act] = set()
            d_exists[act].add(int(line[lastPar+1:lastClose]))
        elif line.startswith("Absence"):
            firstOpen = line.find('(')
            lastClose = line.rfind(')')
            lastPar = line.rfind('§')
            act = line[firstOpen + 1:lastPar - 1].strip()
            acts.add(act)
            if act not in d_absences:
                d_absences[act] = set()
            d_absences[act].add(int(line[lastPar+1:lastClose]))
        elif line.startswith("Init") or line.startswith("End"):
            firstOpen = line.find('(')
            lastClose = line.rfind(')')
            lastPar = line.rfind('§')
            act = line[firstOpen + 1:lastPar - 1].strip()
            acts.add(act)
        else:
            firstOpen = line.find('(')
            lastClose = line.rfind(')')
            comma = line.find(',')
            comma2 = line.rfind(',')
            if (comma == comma2):
                act = line[firstOpen + 1:comma].strip()
                acts.add(act)
                act = line[comma +1:lastClose].strip()
                acts.add(act)
            else:
                sys.exit(1)

from pathlib import Path
path = Path(file)
print("Writing acts....")
with open(os.path.join(path.parent.absolute(), "acts.txt"), "w") as fp:
    fp.write(os.linesep.join(acts))

with open(os.path.join(path.parent.absolute(), "exists.txt"), "w") as fp:
    fp.write(os.linesep.join([" ".join([str(len(d_exists[act]))]+[str(x) for x in d_exists[act]]+[act]) for act in d_exists]))

with open(os.path.join(path.parent.absolute(), "absences.txt"), "w") as fp:
    fp.write(os.linesep.join([" ".join([str(len(d_exists[act]))]+[str(x) for x in d_exists[act]]+[act]) for act in d_absences]))
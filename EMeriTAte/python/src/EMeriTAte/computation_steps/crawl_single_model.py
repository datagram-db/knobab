import glob
import os.path
import sys
from collections import defaultdict
from pathlib import Path

from EMeriTAte.utilities.utils import ForParsing


class ProcessClasses:
    def __init__(self):
        self.d_exists = dict()
        self.d_absences = dict()
        self.acts = set()
        self.files = set()

    def dump(self, folder, rem=None):
        from pathlib import Path
        if isinstance(folder, str):
            path = Path(folder)
        if isinstance(folder, Path):
            path = folder
        else:
            path = Path(folder)
        print("Writing acts....")
        if rem is not None:
            if rem in self.acts:
                self.acts.remove(rem)
            if rem in self.d_exists:
                self.d_exists.pop(rem)
            if rem in self.d_absences:
                self.d_absences.pop(rem)
        with open(os.path.join(path.absolute(), "acts.txt"), "w") as fp:
            fp.write(os.linesep.join(self.acts))
        with open(os.path.join(path.absolute(), "exists.txt"), "w") as fp:
            fp.write(os.linesep.join(
                [" ".join([str(len(self.d_exists[act]))] + [str(x) for x in self.d_exists[act]] + [act]) for act in self.d_exists]))
        with open(os.path.join(path.absolute(), "absences.txt"), "w") as fp:
            fp.write(os.linesep.join(
                [" ".join([str(len(self.d_absences[act]))] + [str(x) for x in self.d_absences[act]] + [act]) for act in self.d_absences]))

    def process(self, file):
        with open(file, "r") as f:
            self.files.add(file)
            for line in f.read().splitlines():
                if line.startswith("Exists"):
                    firstOpen = line.find('(')
                    lastClose = line.rfind(')')
                    lastPar = line.rfind('§')
                    act = line[firstOpen + 1:lastPar - 1].strip()
                    self.acts.add(act)
                    if act not in self.d_exists:
                        self.d_exists[act] = set()
                    self.d_exists[act].add(int(line[lastPar + 1:lastClose]))
                elif line.startswith("Absence"):
                    firstOpen = line.find('(')
                    lastClose = line.rfind(')')
                    lastPar = line.rfind('§')
                    act = line[firstOpen + 1:lastPar - 1].strip()
                    self.acts.add(act)
                    if act not in self.d_absences:
                        self.d_absences[act] = set()
                    self.d_absences[act].add(int(line[lastPar + 1:lastClose]))
                elif line.startswith("Init") or line.startswith("End"):
                    firstOpen = line.find('(')
                    lastClose = line.rfind(')')
                    lastPar = line.rfind('§')
                    act = line[firstOpen + 1:lastPar - 1].strip()
                    self.acts.add(act)
                else:
                    firstOpen = line.find('(')
                    lastClose = line.rfind(')')
                    comma = line.find(',')
                    comma2 = line.rfind(',')
                    if (comma == comma2):
                        act = line[firstOpen + 1:comma].strip()
                        self.acts.add(act)
                        act = line[comma + 1:lastClose].strip()
                        self.acts.add(act)
                    else:
                        print("PARSING ERROR")
                        sys.exit(1)


# file = "/home/giacomo/projects/knobab2_loggen/output_model_healthcare/removed_non_rilevazione/log_weekly.json_1_1_1_0_clazz=1.txt"
#
# def old():
#     pc = ProcessClasses()
#     pc.process(file)
#     from pathlib import Path
#     pc.dump(Path(file).parent.absolute())



def neu():
    filename_fileds = ["mining_supp", "reduction", "isFilenamePolyadic", "reclassify"]
    S = set(["Choice", "RespExistence", "Response", "ChainResponse", "Precedence", "ChainPrecedence", "CoExistence",
             "Succession", "ChainSuccession", "Init", "End", "Exists", "Absence", "Choice", "ExclChoice"])
    root_dir = "/home/giacomo/projects/knobab2_loggen/output_model_healthcare/"
    untimed = set(["Choice", "RespExistence", "CoExistence", "Choice", "ExclChoice"])
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
            if "isFilenamePolyadic" in d:
                d.pop("isFilenamePolyadic")
                d.pop("reclassify")
                d.pop("class")
                d.pop("filename")
                for i in range(3):
                    minsupp_reduction_conf[d["mining_supp"]][d["reduction"]][i] = "ciao"


minsupp_reduction_conf = defaultdict(lambda : defaultdict(lambda : defaultdict(ProcessClasses)))
def yi(file, key, d):
    if "isFilenamePolyadic" in d:
        d.pop("reclassify")
        d.pop("class")
        d.pop("filename")
        minsupp_reduction_conf[d["isFilenamePolyadic"]][d["mining_supp"]][d["reduction"]].process(file)

def dump_txt_files(path):
    fp = ForParsing(path)
    fp.yielder(yi)
    for poly, inner in minsupp_reduction_conf.items():
        NP = "nopoly" if (int(poly)==0 or (not poly)) else "poly"
        for supp, values in inner.items():
            for red, obj in values.items():
                p = Path(os.path.join(path, f"{NP}_s{supp}_{red}"))
                p.mkdir(parents=True, exist_ok=True)
                obj.dump(p, "__missing")
                yield p, supp, poly, red
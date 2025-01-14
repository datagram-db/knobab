import os.path
import sys
from pathlib import Path
from typing import List

import pandas

from EMeriTAte.computation_steps.DTMining import DTMining
from EMeriTAte.computation_steps.crawl_single_model import dump_txt_files
import subprocess
from loguru import logger
from EMeriTAte.computation_steps.learn_from_cpp_csvs import LearnRepresentation

from EMeriTAte.KnoBABEMeriTAteSupport import KnobabEmeritateSupport

class EMeriTAte:

    def __init__(self,  #cpp_binary,
                 environment_field,
                 folder, class_field, time_field, epsilon=0.01, maxval=10000000000000.0, ignore=None, replace=None,
                 conversion=None, toExtendWithTime=None,
                 ignorable_fields=None, support:float|List[float]=0.0, clazz="class", spec=None, criterion="gini", max_depth=5, split=0.3,
                 red=None, polymine=None):
        if polymine is None:
            polymine = True
        if red is None:
            red = False
        self.red = red
        self.polymine = polymine
        self.toExtendWithTime = toExtendWithTime
        self.conversion = conversion
        self.replace = replace
        self.ignore = ignore
        self.maxval = maxval
        self.epsilon = epsilon
        self.time_field = time_field
        self.class_field = class_field
        self.folder = folder
        self.split = split
        self.max_depth = max_depth
        self.criterion = criterion
        self.spec = spec
        self.clazz = clazz
        # self.cpp_binary = str(cpp_binary)
        self.environment_field = environment_field #e.g, user
        self.support = support
        if ignorable_fields is None:
            ignorable_fields = []
        self.ignorable_fields = ignorable_fields
        self.Model = None
        self.resF = os.path.join(self.folder, "__results")
        Path(self.resF).mkdir(parents=True, exist_ok=True)


    def __args(self, folder=None):
        LS = []
        if folder is not None:
            LS = ["-f", str(folder), "-s", str(self.support), "-d", str(self.environment_field)]
        else:
            LS = ["-s", str(self.support), "-d", str(self.environment_field)]
        for x in self.ignorable_fields:
            LS.append('-i')
            LS.append(str(x))
        LS.append("-l")
        LS.append("-p")
        LS.append(str(self.json_path.absolute()))
        return tuple(LS)

    def __run(self, isFastSat, path): #args):
        self.knobab.call_interface(self.time_field, isFastSat, path, self.polymine, self.red)

    def _02_run_preliminary_mining(self, path):
        print(self.__run(False, path))

    def _04_run_fastSAT(self, path):
        print(self.__run(True, path)) #orig:self.__args(path)

    def run_phase1(self):
        import datetime
        #self.file = "/home/giacomo/projects/knobab2_loggen/polyadic_preprocessing/raw_data/log_weekly.json"
        start = datetime.datetime.now()
        logger.trace("A. Specification Mining Phase: TS->json Polyadic Trace")
        self.file = DTMining(self.folder,
             self.class_field,
             self.time_field,
                             self.epsilon,self.maxval,
             self.ignore,
             self.replace,
             self.conversion,
                   self.toExtendWithTime).transform()
        self.json_path = Path(self.file)
        self.knobab = KnobabEmeritateSupport(self.support, self.environment_field, self.file, self.ignorable_fields)
        mining_and_json_ser_ts = datetime.datetime.now()
        mining_and_json_ser = mining_and_json_ser_ts - start
        mining_and_json_ser = mining_and_json_ser.total_seconds() * 1000
        with open(os.path.join(self.resF, "mining_and_join_time.csv"), "a") as f:
            f.write(str(mining_and_json_ser)+os.linesep)
        exit(101)
        #
        # # 02. Bolt2 Specification Mining
        logger.trace("B. Specification Mining Phase: json Polyadic Trace->DECLAREd Specifications")
        self._02_run_preliminary_mining()

        for path, supp, poly, red in dump_txt_files(str(self.json_path.parent.absolute())):
            poly = True if poly == "1" else False
            red = True if red == "1" else False
            supp = float(supp)
            if (supp == self.support) and (poly == self.polymine) and (self.red == red):
                logger.trace(f"C. Specification Mining Phase: DECLAREd Specifications->SAT supp={supp}, poly={poly}, red={red}")
                self._04_run_fastSAT(path)

    def run(self, runs=1):
        self.run_phase1()
        self.run_phase2(runs)

    def run_phase2(self, runs=1):
        import datetime
        f1 = -10000000000000000000000000
        #     Dumping the txt files
        totalResults = []
        self.file = os.path.join(self.folder, "log_weekly.json")
        self.json_path = Path(self.file)

        if self.json_path.exists():
            score = "f1"
            for path, supp, poly, red in dump_txt_files(str(self.json_path.parent.absolute())):
                before_load_data = datetime.datetime.now()

                logger.trace("D. Data loading in python")
                model = LearnRepresentation(str(path), self.clazz, self.spec, self.criterion, self.max_depth, self.split)
                score = "f1" if model.nclasses <= 2 else "macro_f1"
                load_data = datetime.datetime.now() - before_load_data
                load_data = load_data.total_seconds() * 1000

                for _ in range(runs):
                    logger.trace( "E. Ad Hoc explanation+Post Hoc (scores+whitebox)")
                    before_testing = datetime.datetime.now()
                    d = model.test(poly=poly, supp=supp, red=red)
                    testing = datetime.datetime.now() - before_testing
                    testing = testing.total_seconds() * 1000
                    if d is not None:
                        print(d)
                        totalResults.append(d)
                        f1 = max(d[score], f1)
                        d["load_data"] = load_data
            self.Model = [d for d in totalResults if d[score]==f1]
            pandas.DataFrame(totalResults).to_csv(os.path.join(self.resF, "results_proposed.csv"), index=False, mode='a')
        else:
            logger.error("ERROR: the polyadic file doesn't exist")


if __name__ == "__main__":
    logger.remove(0)
    f = sys.stdout
    logger.add(f, level="TRACE")
    # replace = {"fulltime"}
    conversion = ["Ok", "Off"]
    e = EMeriTAte("user", #Collective name for the environment types
                  # Folder containing only .csv files, which file name is the environment
                  #  name
                  "/home/giacomo/projects/knobab2_loggen/polyadic_preprocessing/raw_data",
        # CSV column containing the class information [0,1]          
        "event",
        # CSV column containing the timestamp information
        "fulltime",
        # \epsilon parameter
                  0.01,
        # Arbitrary large number          
                  10000000000000.0,
        # Fields not to be considered for the mining          
                  {'amantadine', 'day', 'madopar 50', 'madopar CR', 'rotigotine', 'span', 'stalevo', 'stanek'},
                  None,
        # Converts the number from the CSV classes into labels          
                  conversion,
                  None,
        #Fields to be ignored while loading the data           
                  ["day", "span", "__class", "__label","fulltime"],
        # Support mining          
                  0.0)
    e.run()

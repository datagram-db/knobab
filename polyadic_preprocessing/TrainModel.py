import sys
from pathlib import Path

from computation_steps.DTMining import DTMining
from computation_steps.crawl_single_model import dump_txt_files
import subprocess
from loguru import logger
from computation_steps.learn_from_cpp_csvs import LearnRepresentation

from KnoBABEMeriTAteSupport import KnobabEmeritateSupport

class EMeriTAte:

    def __init__(self, #cpp_binary,
                 environment_field,
                 folder, class_field, time_field, epsilon=0.01, maxval=10000000000000.0, ignore=None, replace=None,
                 conversion=None, toExtendWithTime=None,
                 ignorable_fields=None, support=0.0, clazz="class", spec=None, criterion="gini", max_depth=5, split=0.3):
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

    def __run(self, path=None): #args):
        # self.knobab.call_interface(path)
        print(path)
        LS = list(self.__args(path))
        LS.insert(0, "/home/giacomo/projects/knobab2_loggen/cmake-build-release/knobab_json")
        popen = subprocess.Popen(tuple(LS), stdout=subprocess.PIPE)
        popen.wait()
        return popen.stdout.read()

    def _02_run_preliminary_mining(self):
        print(self.__run())

    def _04_run_fastSAT(self, path):
        print(self.__run(path)) #orig:self.__args(path)

    def run(self):
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

        # 02. Bolt2 Specification Mining
        self._02_run_preliminary_mining()
        #     Dumping the txt files
        iterable = list(dump_txt_files(str(self.json_path.parent.absolute())))
        assert len(iterable) == 1

        # 04. Deviance Learning
        path = iterable[0]
        self._04_run_fastSAT(path)
        model = LearnRepresentation(str(path), self.clazz, self.spec, self.criterion, self.max_depth, self.split)
        model.train()
        self.Model = model.rf



if __name__ == "__main__":
    logger.remove(0)
    f = sys.stdout
    logger.add(f, level="TRACE")
    replace = {"fulltime":"time"}
    conversion = ["Ok", "Off"]
    e = EMeriTAte(#"/home/giacomo/projects/knobab2_loggen/cmake-build-release/knobab_json",
                  "user",
                  "/home/giacomo/projects/knobab2_loggen/polyadic_preprocessing/raw_data",
        "event",
        "time",
                  0.01,
                  10000000000000.0,
                  {'amantadine', 'day', 'madopar 50', 'madopar CR', 'rotigotine', 'span', 'stalevo', 'stanek'},
                  replace,
                  conversion,
                  {'amantadine', 'madopar 50', 'madopar CR', 'rotigotine', 'stalevo', 'stanek'},
                  ["day", "span", "__class", "__label","time","fulltime"],
                  0.0)
    e.run()
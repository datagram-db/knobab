import time

from EMeriTAte.comsis25.FirstPhase import mining
from loguru import logger
from dataclasses import dataclass

@dataclass
class HyperParameters:
    phase_1_benchmark_json_file: str
    phase_1_epsilon: float = 0.01
    phase_1_maxval : float = 10000000000000.0


def new_pipeline(csv_folder:str,
            isDataless:bool,
           algorithm:int,
           benchmark_json_file,
            epsilon:float=0.01,
           maxval:float=10000000000000.0,
           **kwargs):
    pass





if __name__ == "__main__":
    benchmark_file = "benchmark_dyskinetic.json"
    # benchmark_file = "benchmark_italy.json"
    for i in range(5):
        logger.info(f"Run #{i}")
        logger.info("Algo 1 running")
        mining("dyskinetic", True, 1, benchmark_file, numclazzes=2)
    #     mining("italy_power_demand", True, 1, benchmark_file, numclazzes=2)
        # logger.info("Algo 2 [Dataless] running")
        # mining("italy_power_demand", True, 2, benchmark_file, numclazzes=2)
        # logger.info("Algo 3 [Dataless] running")
        # mining("italy_power_demand", True, 3, benchmark_file, numclazzes=2)
        # logger.info("Algo 2 [Dataful] running")
        # mining("italy_power_demand", False, 2, benchmark_file, numclazzes=2)
        # logger.info("Algo 3 [Dataful] running")
        # mining("italy_power_demand", False, 3, benchmark_file, numclazzes=2)
    # benchmark_file = "benchmark_japanese.json"
    # for i in range(5):
    #     logger.info(f"Run #{i}")
    #     logger.info("Algo 1 running")
    #     mining("japanese_vowels", True, 1, benchmark_file, numclazzes=9)
    #
    # benchmark_file = "benchmark_load_basic_motions.json"
    # for i in range(5):
    #     logger.info(f"Run #{i}")
    #     logger.info("Algo 1 running")
    #     mining("load_basic_motions", True, 1, benchmark_file, numclazzes=4)
    #
    # benchmark_file = "benchmark_osuleaf.json"
    # for i in range(5):
    #     logger.info(f"Run #{i}")
    #     logger.info("Algo 1 running")
    #     mining("osuleaf", True, 1, benchmark_file, numclazzes=6)



import json, os
import knobab_emeritate_support
from datetime import datetime
from dataclasses import dataclass, field

from EMeriTAte.computation_steps.DTMining import DTMining

@dataclass
class FirstPhaseBenchmarkJson:
    experiment_number: int
    folder: str
    precision: float
    maxval: float
    loading: float ## loading time
    serial: float ## serialization time
    Algo: str
    total_time_mining: float
    total_event_time_serial: float
    total_time: float
    all_stats: list = field(default_factory=list)


def mining(csv_folder:str,
            isDataless:bool,
           algorithm:int,
           benchmark_json_file,
            epsilon:float=0.01,
           maxval:float=10000000000000.0,
           **kwargs
           ):
    file = None
    ## 1. The first algorithm (full pedantic one from the IDEAS24 paper). As the dataful version (catch24) was too costly, it was abandoned.
    ## 2. First version of the algorithm, improving slightly over the first version of the algorithm. This distinguishes between dataful and dataless
    ## 3. Second version of the algorithm, allegedly requiring less memory to processing data. This distinguishes between dataful and dataless
    if (algorithm == 1):
        experiment_number = 0
        preliminary_data = []
        if os.path.exists(benchmark_json_file):
            with open(benchmark_json_file, "r") as json_file:
                preliminary_data = json.load(json_file)
                assert isinstance(preliminary_data, list)
                experiment_number = len(preliminary_data)
        ## Calling the legacy python pipeline
        clazz = "class"
        timef = "time"
        ignore = set(kwargs.get("ignore",set()))
        numclazzes = int(kwargs.get("numclazzes",-1))
        if numclazzes>-1:
            conversion = [str(x) for x in range(numclazzes)]
        else:
            conversion = None
        ignorable = kwargs.get("ignorable",None)

        first_miner = DTMining(csv_folder, clazz, timef, epsilon,maxval,
                        ignore, None, conversion, None)

        start = datetime.datetime.now()
        file = first_miner.transform("polyadic_Algo1_dataless.json")
        mining_and_json_ser_ts = datetime.datetime.now()
        mining_and_json_ser = mining_and_json_ser_ts - start
        milliseconds = mining_and_json_ser.total_seconds() * 1000

        total_time = first_miner.loading_time + first_miner.serial_time + first_miner.mine_time + first_miner.event_serial_time
        benchmark_result = FirstPhaseBenchmarkJson(experiment_number, csv_folder, epsilon, maxval, first_miner.loading_time, first_miner.serial_time, "Algorithm1", first_miner.mine_time, first_miner.event_serial_time, total_time)
        preliminary_data.append(benchmark_result)
        with open(benchmark_json_file, "w") as json_file:
            json.dump(preliminary_data, json_file)


    else:
        assert isinstance(algorithm, int) and ((algorithm == 2) or (algorithm == 3))
        ### Run the C++ code
        file = knobab_emeritate_support.dt_mine_and_ts_to_polyadic(benchmark_json_file, csv_folder, epsilon, maxval, algorithm==3, isDataless)
    return file
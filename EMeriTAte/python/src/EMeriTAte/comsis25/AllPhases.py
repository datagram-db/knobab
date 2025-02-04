import os.path


def EMeriTAte(csv_folder:str,
            isDataless:bool,
           algorithm_first_phase:int,
              algorithm_second_phase:int,
           benchmark_json_file = None,
            epsilon:float=0.01,
           maxval:float=10000000000000.0,
              dataset_name = None,
              nranges=10,
           **kwargs):
    from EMeriTAte.comsis25 import FirstPhase
    from EMeriTAte.comsis25 import SecondPhase
    from EMeriTAte.comsis25 import ThirdPhase


    assert os.path.isdir(csv_folder)
    if dataset_name is None:
        dataset_name = os.path.basename(csv_folder)
    if benchmark_json_file is None:
        benchmark_json_file = os.path.join(csv_folder, f"benchmark_{dataset_name}.json")
    numclasses = kwargs.get("numclasses", 2)
    ignorable = kwargs.get("ignorable", None)
    time_field = kwargs.get("time_field", "time")
    environment_field = kwargs.get("environment_field", "user")
    support = kwargs.get("support", 0.0)
    reduce = kwargs.get("reduce", False)
    polymine = kwargs.get("polymine", True)
    class_field = kwargs.get("class_field", "class")
    split = kwargs.get("split", 0.3)
    criterion = kwargs.get("criterion", "gini")
    max_depth = kwargs.get("max_depth", 5)

    file = FirstPhase.mining(csv_folder, isDataless, algorithm_first_phase, benchmark_json_file, epsilon, maxval, **kwargs)
    pandas_dataset = SecondPhase.knowledge_extraction_algorithm(file, algorithm_second_phase, **kwargs)
    ThirdPhase.training(dataset_name, pandas_dataset, algorithm_first_phase, algorithm_second_phase, polymine, support, reduce, nranges, numclasses, split, class_field, criterion, max_depth)

if __name__ == "__main__":
    dys = "/home/giacomo/projects/knobab2_loggen/EMeriTAte/dyskinetic"
    ita = "/home/giacomo/projects/knobab2_loggen/EMeriTAte/italy_power_demand"
    jap = "/home/giacomo/projects/knobab2_loggen/EMeriTAte/japanese_vowels"
    bm = "/home/giacomo/projects/knobab2_loggen/EMeriTAte/load_basic_motions"
    isDataless = True
    algorithm_first_phase = 1
    algorithm_second_phase = 4

    #EMeriTAte(dys, True, algorithm_first_phase, algorithm_second_phase, support=1.0, numclasses=2)
    #EMeriTAte(ita, True, algorithm_first_phase, algorithm_second_phase, support=0.0, numclasses=2)
    # EMeriTAte(dys, True, algorithm_first_phase, algorithm_second_phase, support=1.0, numclasses=2)
    EMeriTAte(jap, True, algorithm_first_phase, algorithm_second_phase, support=1.0, numclasses=9)
    # EMeriTAte(bm, True, algorithm_first_phase, algorithm_second_phase, support=0, numclasses=4)
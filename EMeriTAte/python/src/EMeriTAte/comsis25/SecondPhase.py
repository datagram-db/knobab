import os.path
import re
import shutil
from collections import defaultdict
from os import listdir
from os.path import isfile, join
from pathlib import Path

import pandas

from EMeriTAte.computation_steps.learn_from_cpp_csvs import loadDataset



def neu_pipeline(poly_folder,
                 clazzName='class'):
    dir = os.path.join(poly_folder, "dataful_comsis")
    assert os.path.isdir(dir)
    init_match = r"^Refinement\_init\_payload\_(\d+)\.csv$"
    exists_match = r"^Refinement\_exists\_.+\_payload_(\d+)\.csv$"
    end_match = r"^Refinement\_end\_.+\_payload_(\d+)\.csv$"
    dataless_match = r"output\_csv\_(\d)+\.csv"
    dataful_match = r"output\_csv\_(\d)+\_dataful\_\.csv"

    payload_colfeat_tracerow = defaultdict(list)
    class_no_traces = dict()
    for f in sorted(listdir(dir)):
        if os.path.isfile(os.path.join(dir, f)):
            init = re.match(init_match, f)
            if init:
                clazz = init.group(1)
                df = pandas.read_csv(os.path.join(dir, f))
                if clazz not in class_no_traces:
                    class_no_traces[clazz] = len(df.index)
                else:
                    assert class_no_traces[clazz] == len(df.index)
                payload_colfeat_tracerow[clazz].append(df.transpose())
            else:
                exists = re.match(exists_match, f)
                if exists:
                    clazz = exists.group(1)
                    df = pandas.read_csv(os.path.join(dir, f))
                    if clazz not in class_no_traces:
                        class_no_traces[clazz] = len(df.index)
                    else:
                        assert class_no_traces[clazz] == len(df.index)
                    payload_colfeat_tracerow[clazz].append(df.transpose())
                else:
                    ends = re.match(end_match, f)
                    if ends:
                        clazz = ends.group(1)
                        df = pandas.read_csv(os.path.join(dir, f))
                        if clazz not in class_no_traces:
                            class_no_traces[clazz] = len(df.index)
                        else:
                            assert class_no_traces[clazz] == len(df.index)
                        payload_colfeat_tracerow[clazz].append(df.transpose())
                    else:
                        dataless = re.match(dataless_match, f)
                        if dataless:
                            clazz = dataless.group(1)
                            df = pandas.read_csv(os.path.join(dir, f), index_col=0, header=None)
                            payload_colfeat_tracerow[clazz].append(df)
                            N = len(df.transpose().index)
                            if clazz not in class_no_traces:
                                class_no_traces[clazz] = N
                            else:
                                assert class_no_traces[clazz] == N
                        else:
                            dataful = re.match(dataful_match, f)
                            if dataful:
                                clazz = dataful.group(1)
                                df = pandas.read_csv(os.path.join(dir, f), index_col=0, header=None)
                                payload_colfeat_tracerow[clazz].append(df)
                                if clazz not in class_no_traces:
                                    class_no_traces[clazz] = N
                                else:
                                    assert class_no_traces[clazz] == N
    payload_colfeat_tracerow = [(clazz, pandas.concat(map(lambda x : x.transpose().reset_index(drop=True, inplace=False), dfs), axis=1)) for clazz, dfs in payload_colfeat_tracerow.items()]
    return loadDataset(payload_colfeat_tracerow, clazzName, False)


def old_pipeline(json_file, numclasses,
                 polymine:bool=True,
                 reduce:bool=False,
                 support = 0.0,
                 environment_field = "user",
                 time_field = "time",
                 reclassify = False,
                 ignorable = None,
                 class_field = "class",
                 regex = r"output\_csv\_(\d)+\.csv"):
    from EMeriTAte.KnoBABEMeriTAteSupport import KnobabEmeritateSupport
    from EMeriTAte.computation_steps.crawl_single_model import ProcessClasses, dump_txt_files
    from EMeriTAte.utilities.utils import ForParsing
    from EMeriTAte.computation_steps.crawl_single_model import yi

    if ignorable is None:
        ignorable = ["day", "span", "__class", "__label", "fulltime"]

    polyint = 1 if polymine else 0
    poly_var = "nopoly" if ((not polymine)) else "poly"
    red = 1 if reduce else 0
    supp = 0 if (support == 0.0) else (1 if (support == 1.0) else support)
    desired_folder = f"{poly_var}_s{supp}_{red}"
    rec = 1 if reclassify else 0
    json_algo4_dir = os.path.join(Path(json_file).parent.absolute(), Path(json_file).stem + "_algo4")
    if not (os.path.isdir(json_algo4_dir)) and not (os.path.exists(json_algo4_dir)):
        os.mkdir(json_algo4_dir)
    dst_json = os.path.join(json_algo4_dir, Path(json_file).name)
    if not os.path.isfile(dst_json) and not (os.path.exists(dst_json)):
        shutil.copyfile(json_file, dst_json)
    pp = os.path.join(json_algo4_dir, desired_folder)

    knobab = KnobabEmeritateSupport(support, environment_field, dst_json, ignorable, reclassify)
    knobab.call_interface(time_field, False, json_algo4_dir if not reclassify else pp, polymine, reduce)

    for i in range(numclasses):
        pc = ProcessClasses()
        pc.process(dst_json + f"_{supp}_{red}_{polyint}_{rec}_clazz={i}.txt")
        p = Path(pp)
        p.mkdir(parents=True, exist_ok=True)
        pc.dump(p, "__missing")
    class_files = []

    if not reclassify:
        knobab.call_interface(time_field, True, pp, polymine, reduce)
        pp, final = pp, dst_json + f"_{supp}_{red}_{polyint}_{rec}_clazz={{clazz}}.txt"

        ## Reading all of the class files that have been dumped
        for f in [f for f in listdir(pp) if isfile(join(pp, f))]:
            m = re.search(regex, f)
            if m is not None:
                clazz = int(m.group(1))
                txt = final.format(clazz=clazz)
                class_files.append((clazz, os.path.join(pp, f)))
                assert os.path.isfile(txt)
        ## Loading all of the datasets belonging to that class
        return loadDataset(class_files, class_field)
    else:
        print("TODO!!!")




def knowledge_extraction_algorithm(json_file_from_first_phase: str,
                                   algorithm: int,
                                   **kwargs) -> pandas.DataFrame:
    if algorithm == 4:
        numclasses = kwargs.get("numclasses", 2)
        ignorable = kwargs.get("ignorable", None)
        time_field = kwargs.get("time_field", "time")
        environment_field = kwargs.get("environment_field", "user")
        support = kwargs.get("support", 0.0)
        reduce = kwargs.get("reduce", False)
        polymine = kwargs.get("polymine", True)
        return old_pipeline(json_file_from_first_phase, numclasses, polymine, reduce, support, environment_field, time_field, False, ignorable)
    elif algorithm == 5:
        numclasses = kwargs.get("numclasses", 2)
        ignorable = kwargs.get("ignorable", None)
        time_field = kwargs.get("time_field", "time")
        environment_field = kwargs.get("environment_field", "user")
        support = kwargs.get("support", 0.0)
        reduce = kwargs.get("reduce", False)
        polymine = kwargs.get("polymine", True)
        return old_pipeline(json_file_from_first_phase, numclasses, polymine, reduce, support, environment_field,
                            time_field, True, ignorable)



if __name__ == "__main__":
    d = neu_pipeline("/media/giacomo/Data/osuleaf/test/")
    print(d)
    # knowledge_extraction_algorithm("/home/giacomo/projects/knobab2_loggen/EMeriTAte/italy_power_demand/polyadic_Algo1_dataless.json", 5)
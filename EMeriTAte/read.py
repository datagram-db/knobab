import json
import pandas

if __name__ == '__main__':
    # import os, fnmatch
    # path = '/home/giacomo/projects/knobab2_loggen/EMeriTAte/japanese_vowels'
    # for file in fnmatch.filter(os.listdir(path), '*.csv'):
    #     full = os.path.join(path, file)
    #     df = pandas.read_csv(full)
    #     vals = df.drop(["class", "time"], axis=1)
    #     res = vals.index[vals.isnull().all(1)]
    #     df.drop(res, inplace=True)
    #     df.to_csv(full, index=False)
    # with open('/home/giacomo/projects/knobab2_loggen/EMeriTAte/osuleaf/polyadic_Algo3_dataless.json', 'r', encoding='utf-8') as f:
    #     data3 = json.load(f)
    with open('/home/giacomo/projects/knobab2_loggen/EMeriTAte/test/polyadic_Algo3_dataful.json', 'r', encoding='utf-8') as f:
        data1 = json.load(f)
    print(data1)
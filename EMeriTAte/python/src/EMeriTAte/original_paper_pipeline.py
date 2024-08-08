import types

import yaml
from .original_paper.medical_analysis import MedicalAnalysis
from yaml.loader import SafeLoader

if __name__ == '__main__':
    conf = None
    with open("Configuration.yaml", "r") as f:
        conf = yaml.load(f, Loader=SafeLoader)
        conf = types.SimpleNamespace(**conf)
    app = MedicalAnalysis(conf)
    # app.dump_raw_data_as_csv()
    app.all_patients_analysis()

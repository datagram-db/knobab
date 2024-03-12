import types

import yaml
from medical_analysis import MedicalAnalysis
from yaml.loader import SafeLoader

if __name__ == '__main__':
    conf = None
    with open("Configuration.yaml", "r") as f:
        conf = yaml.load(f, Loader=SafeLoader)
        conf = types.SimpleNamespace(**conf)
    app = MedicalAnalysis(conf)
    app.all_patients_analysis()
    # app.singlePatientWeekly()
    # app.singlePatientDaily()
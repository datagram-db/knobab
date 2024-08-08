import datetime
import json
import math

class SubstanceOrActiveInData:
    def __init__(self, amount, time):
        self.amount = amount
        self.time = time

class SubstanceTime:
    def __init__(self, name, half_life, epsilon_th=0.01, amount = 0, start_time = datetime.datetime.min):
        self.name = name
        assert isinstance(half_life, datetime.timedelta)
        self.half_life = half_life
        self.epsilon_th = epsilon_th
        self.amount = amount
        self.start_time = start_time

    def __call__(self, time):
        assert isinstance(time, datetime.datetime)
        if (time <= self.start_time):
            return 0
        t = (time - self.start_time).total_seconds()
        hl = self.half_life.total_seconds()
        val = self.amount * (2 ** (-t/hl))
        if val < self.epsilon_th:
            return 0
        else:
            return val


class SubstanceAssumptionInTime:
    def __init__(self, assumptions : list[SubstanceTime]):
        self.assumptions = assumptions

    def __call__(self, time):
        assert isinstance(time, datetime.datetime)
        return sum(map(lambda x : x(time), self.assumptions))


class ActivePrinciple:
    def __init__(self, name:str, half_life, equivalent_dose, epsilon_th):
        self.name = name
        self.half_life = half_life
        self.equivalent_dose = equivalent_dose
        self.epsilon_th = epsilon_th

    def fromAssumptionInTime(self, dose, perDoseUnit, time):
        return SubstanceTime(self.name, self.half_life, self.epsilon_th, dose / perDoseUnit * self.equivalent_dose, time)

    def fromAssumptionsInTime(self, perDoseUnit, assumptions:list[SubstanceOrActiveInData]):
        return SubstanceAssumptionInTime([self.fromAssumptionInTime(x.amount, perDoseUnit, x.time) for x in assumptions])

class RunningSubstance:
    def __init__(self, dicts):
        self.dicts = dicts

    def inRunningTime(self, timeList):
        return {k: [self.dicts[k](time) for time in timeList] for k in self.dicts.keys()}

class Substance:
    def __init__(self, name:str, active_principles: list[ActivePrinciple], per_dose_unit:float):
        self.name = name
        self.active_principles = {k.name:k for k in active_principles}
        self.per_dose_unit = per_dose_unit

    def fromAssumptionInTime(self, assumptions:list[SubstanceOrActiveInData]):
        return RunningSubstance({k: self.active_principles[k].fromAssumptionsInTime(self.per_dose_unit, assumptions) for k in self.active_principles.keys()})


class Chemists:
    def __init__(self, json_file_configuration):
        #data = {}
        with open(json_file_configuration, "r") as file:
            data = json.load(file)
        self.dToResolve = dict()
        start_time_data = datetime.datetime.now()  # Bogus
        self.repository = {}
        for key, val in data.items():
            if key == "_comment":
                continue
            for x in val["synonyms"]:
                self.dToResolve[x] = key
            per_dose_unit = val["active_ingredient"]["per_dose_unit"]
            AP = []
            for name, field in val["active_ingredient"]["composition"].items():
                half_life = datetime.timedelta(hours=field["half-life"])
                equivalent_dose = field["equivalent_dose"]
                epsilon_th = 0.01
                if "total" in field:
                    ## Estimating the minimum threshold from this
                    s = SubstanceTime(name, half_life, amount=equivalent_dose, start_time=start_time_data)
                    delta_end = datetime.timedelta(hours=field["total"])
                    time = start_time_data + delta_end
                    epsilon_th = s(time)
                AP.append(ActivePrinciple(name, half_life, equivalent_dose, epsilon_th))
            self.repository[key] = Substance(key, AP, per_dose_unit)

    def resolveName(self,name):
        if name not in self.dToResolve:
            if name in self.dToResolve.values():
                return name
            else:
                return None
        else:
            return self.dToResolve[name]

    def retrieve(self, name):
        if name not in self.repository:
            return None
        else:
            return self.repository[name]

    def resolve(self, name):
        if name not in self.dToResolve:
            if name in self.dToResolve.values():
                return self.repository[name]
            else:
                return None
        else:
            return self.repository[self.dToResolve[name]]

if __name__ == "__main__":
    data = {}
    with open("i_want_a_new_drug.json", "r") as file:
        data = json.load(file)
    dToResolve = dict()
    start_time_data = datetime.datetime.now() #Bogus
    L = [] # list[SubstanceInData]
    repository = {}
    for key, val in data.items():
        if key == "_comment":
            continue
        for x in val["synonyms"]:
            dToResolve[x] = key
        per_dose_unit = val["active_ingredient"]["per_dose_unit"]
        AP = []
        for name, field in  val["active_ingredient"]["composition"].items():
            half_life = datetime.timedelta(hours=field["half-life"])
            equivalent_dose = field["equivalent_dose"]
            epsilon_th = 0.01
            if "total" in field:
                ## Estimating the minimum threshold from this
                s = SubstanceTime(name, half_life, amount=equivalent_dose, start_time=start_time_data)
                delta_end = datetime.timedelta(hours=field["total"])
                time = start_time_data+delta_end
                epsilon_th = s(time)
            AP.append(ActivePrinciple(name, half_life, equivalent_dose, epsilon_th))
        repository[key] = Substance(key, AP, per_dose_unit)
    print(data)
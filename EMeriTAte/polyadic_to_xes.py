import sys
import json

from opyenxes.data_out.XesXmlSerializer import XesXmlSerializer
from opyenxes.factory.XFactory import XFactory

if __name__ == "__main__":
    if len(sys.argv)>1:
        file = sys.argv[1]
        with open(file, "r") as f:
            poly = json.load(f)
            if "log" in poly:
                poly = poly["log"]
                xes_log = XFactory.create_log()
                print(poly)
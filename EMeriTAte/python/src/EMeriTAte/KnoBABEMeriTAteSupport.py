from typing import List

import knobab_emeritate_support

#bool reclassify,
                             # bool reduction,
                             # double mining_supp,
                             # bool isFilenamePolyadic,
                             # const std::string &traceDistinguisher,
                             # const std::string &filename_polyadic,
                             # const std::vector<std::string> &ignore_keys,
# bool isFastSat,
                             # const std::vector<std::string> &worlds_format_to_load,
                             # const std::vector<std::string> &worlds_file_to_load,
                             # const std::string &folder

def _call_original(mining_supp:float,
                   isFilenamePolyadic:bool,
                   traceDistinguisher:str,
                   filename_polyadic:str,
                   ignore_keys:List[str],
                   isFastSat:bool,
                   folder:str,
                   time:str,
                   reduction:bool=False):
    """
    Calling the original C++ function from python
    """
    knobab_emeritate_support.knobab_for_emeritate(False,
                   reduction,
                   mining_supp,
                   isFilenamePolyadic,
                   traceDistinguisher,
                   filename_polyadic,
                   ignore_keys,
                   isFastSat,
                   [],
                   [],
                   folder,
                   time)

from collections.abc import Iterable

class KnobabEmeritateSupport():

    def __init__(self, support:float, environment_field:str, polyadic_file:str, ignorable_fields=None):
        self.json_path = str(polyadic_file)
        self.environment_field = str(environment_field)
        self.support = float(support)
        self.ignore_fields = []
        if ignorable_fields is not None:
            self.ignore_fields = list(set(ignorable_fields))

    def call_interface(self, time, folder = None, isPolyadicMine=True, reduction=False):
        isFastSat = False
        if folder is not None:
            isFastSat = True
        else:
            folder = ""
        _call_original(self.support, isPolyadicMine, self.environment_field, self.json_path, self.ignore_fields, isFastSat, str(folder), time, reduction)



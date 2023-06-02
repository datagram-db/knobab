# KnoBAB v2.0

## Log Generator

This specific version of KnoBAB is the one containing the version for the log generator as presented to the GRADES-NDA'23 Workshop. At this stage, the log generator is not part of the KnoBAB pipeline, which supports an earlier version of the algorithm. The log generator is then associated to the program named `log_generator`, which main function is defined in `generate_traces.cpp`.


```
  /home/giacomo/projects/knobab2/cmake-build-debug/log_generator {OPTIONS}
    [cache] [declare] [sigma] [csv]

    This is KnoBAB's Log Generator.

  OPTIONS:

      -h, --help                        Display this help menu
      cache                             The position where the cache path
                                        resides, where each single automata
                                        refers to one declarative clause
      declare                           The file describing the declarative
                                        model from which extract the clauses
      sigma                             The set of (non-atomised) symbols to be
                                        associated to the model. If the model is
                                        atomised, these sets is expanded with
                                        the corresponding full set of atomised
                                        symbols
      csv                               The CSV file where to dump the result of
                                        the experiments
      -m[minL]                          The minimum trace length
      -M[maxL]                          The maximum trace length
      -L[L]                             The number of distinct traces in the log
      -s[sample]                        The sample ratio for the preliminar
                                        random walk
      This group is all exclusive:
        -t, --tab                         Serialising in a tab separated format
        -x, --xes                         Serialising in a XES format
        -N, --no-serialize                Skips the serialization part
      "--" can be used to terminate flag options and force all following
      arguments to be treated as positional options

    This log generator exploits the algorithm showcased at GRADES-NDA'23.
```

## Cloning this branch

The suggested command to clone this project alongside its submodules and dependencies is the following:

    git clone --recurse-submodules -j8 --branch v2.0 git@github.com:datagram-db/knobab.git knobab2

## Windows
The present project can be compiled with the Visual Studio toolchain via CMake configuration. 

    cmake.exe -DCMAKE_BUILD_TYPE=<Debug|Release> -G "CodeBlocks - NMake Makefiles" -S <source> -B <build-folder>

Then, the process can be compiled as follows:

    cmake.exe <build-folder> --target all

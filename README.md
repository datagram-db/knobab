# KnoBAB v2.0

## Log Generator

See loggen2 branch 

## Mining+Inference Algorithm

**TODO**

## Cloning this branch

The suggested command to clone this project alongside its submodules and dependencies is the following:

    git clone --recurse-submodules -j8 --branch v2.0 git@github.com:datagram-db/knobab.git knobab2

## Windows
The present project can be compiled with the Visual Studio toolchain via CMake configuration. 

    cmake.exe -DCMAKE_BUILD_TYPE=<Debug|Release> -G "CodeBlocks - NMake Makefiles" -S <source> -B <build-folder>

Then, the process can be compiled as follows:

    cmake.exe <build-folder> --target all

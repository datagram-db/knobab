#!/usr/bin/env bash
rm -rf parall
mkdir parall
pushd parall
cmake -DCMAKE_BUILD_TYPE=Release -DPARALLEL=ON .. 
make -j10 knobab-v1.0
popd

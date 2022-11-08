#!/usr/bin/env bash
cd ..
cd ..

rm -rf parall
mkdir parall
pushd parall
cmake -DCMAKE_BUILD_TYPE=Release -DPARALLEL=ON .. 
make -j10 knobab-v1.0
popd

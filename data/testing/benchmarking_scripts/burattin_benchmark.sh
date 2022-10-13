#!/bin/bash
cd ..
cd ..
cd ..
./cmake-build-release/knobab-v1.0 --nostats --xes=data/testing/bpic_2012/data/original/bpi12_knobab.xes --maxsat=scripts/intersection_pipeline.yaml --declareFile=data/testing/bpic_2012/models/5.powerdecl --csv=data/testing/results/knobab_burattin_benchmark.csv --queryCount=5
./cmake-build-release/knobab-v1.0 --nostats --xes=data/testing/bpic_2012/data/original/bpi12_knobab.xes --maxsat=scripts/intersection_pipeline.yaml --declareFile=data/testing/bpic_2012/models/10.powerdecl --csv=data/testing/results/knobab_burattin_benchmark.csv --queryCount=5
./cmake-build-release/knobab-v1.0 --nostats --xes=data/testing/bpic_2012/data/original/bpi12_knobab.xes --maxsat=scripts/intersection_pipeline.yaml --declareFile=data/testing/bpic_2012/models/15.powerdecl --csv=data/testing/results/knobab_burattin_benchmark.csv --queryCount=5
./cmake-build-release/knobab-v1.0 --nostats --xes=data/testing/bpic_2012/data/original/bpi12_knobab.xes --maxsat=scripts/intersection_pipeline.yaml --declareFile=data/testing/bpic_2012/models/20.powerdecl --csv=data/testing/results/knobab_burattin_benchmark.csv --queryCount=5
./cmake-build-release/knobab-v1.0 --nostats --xes=data/testing/bpic_2012/data/original/bpi12_knobab.xes --maxsat=scripts/support_pipeline.yaml --declareFile=data/testing/bpic_2012/models/5.powerdecl --csv=data/testing/results/knobab_burattin_benchmark.csv --queryCount=5
./cmake-build-release/knobab-v1.0 --nostats --xes=data/testing/bpic_2012/data/original/bpi12_knobab.xes --maxsat=scripts/support_pipeline.yaml --declareFile=data/testing/bpic_2012/models/10.powerdecl --csv=data/testing/results/knobab_burattin_benchmark.csv --queryCount=5
./cmake-build-release/knobab-v1.0 --nostats --xes=data/testing/bpic_2012/data/original/bpi12_knobab.xes --maxsat=scripts/support_pipeline.yaml --declareFile=data/testing/bpic_2012/models/15.powerdecl --csv=data/testing/results/knobab_burattin_benchmark.csv --queryCount=5
./cmake-build-release/knobab-v1.0 --nostats --xes=data/testing/bpic_2012/data/original/bpi12_knobab.xes --maxsat=scripts/support_pipeline.yaml --declareFile=data/testing/bpic_2012/models/20.powerdecl --csv=data/testing/results/knobab_burattin_benchmark.csv --queryCount=5

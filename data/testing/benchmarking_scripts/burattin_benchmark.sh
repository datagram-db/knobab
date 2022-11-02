#!/bin/bash
cd ..
cd ..
cd ..

declare -a models=("data/testing/bpic_2012/models/5.powerdecl" "data/testing/bpic_2012/models/10.powerdecl" "data/testing/bpic_2012/models/15.powerdecl" "data/testing/bpic_2012/models/20.powerdecl" "data/testing/bpic_2012/models/model1.powerdecl" "data/testing/bpic_2012/models/model2.powerdecl" "data/testing/bpic_2012/models/model3.powerdecl" "data/testing/bpic_2012/models/model4.powerdecl" "data/testing/bpic_2012/models/model5.powerdecl" "data/testing/bpic_2012/models/model1+2.powerdecl" "data/testing/bpic_2012/models/model1+2+3+4+5.powerdecl" "data/testing/bpic_2012/models/model1+2+4.powerdecl" "data/testing/bpic_2012/models/model1+2+5.powerdecl" "data/testing/bpic_2012/models/model1+3+4.powerdecl" "data/testing/bpic_2012/models/model1+3+5.powerdecl")

declare -a pipelines=("scripts/intersection_pipeline.yaml" "scripts/support_pipeline.yaml")

for model in "${models[@]}"; do
	for pipeline in "${pipelines[@]}"; do
		./cmake-build-release/knobab-v1.0 --nostats --xes=data/testing/bpic_2012/data/original/bpi12_knobab.xes --maxsat="$pipeline" --declareFile="$model" --csv=data/testing/results/benchmarking/burattin.csv --queryCount=5
	done
done

#!/bin/bash
cd ..
cd ..
cd ..

declare -a models=("data/testing/bpic_2012/models/5.powerdecl" "data/testing/bpic_2012/models/10.powerdecl" "data/testing/bpic_2012/models/15.powerdecl" "data/testing/bpic_2012/models/20.powerdecl")

declare -a pipelines=("scripts/atomization_pipeline.yaml" "scripts/atomization_always_pipeline.yaml")

for model in "${models[@]}"; do
	for pipeline in "${pipelines[@]}"; do
		./cmake-build-release/knobab-v1.0 --nostats --atomization="$pipeline"  --xes=data/testing/bpic_2012/data/original/bpi12_knobab.xes --maxsat=scripts/maxsat_pipeline.yaml --declareFile="$model" --csv=data/testing/results/benchmarking/phi.csv --queryCount=5
	done
done

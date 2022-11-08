#!/bin/bash
cd ..
cd ..

declare -a models=("data/bpic_2012/models/scenario_1/5.powerdecl" "data/bpic_2012/models/scenario_1/10.powerdecl" "data/bpic_2012/models/scenario_1/15.powerdecl" "data/bpic_2012/models/scenario_1/20.powerdecl")

declare -a pipelines=("scripts/atomization_pipeline.yaml" "scripts/atomization_always_pipeline.yaml")

for model in "${models[@]}"; do
	for pipeline in "${pipelines[@]}"; do
		./cmake-build-release/knobab-v1.0 --nostats --atomization="$pipeline" --xes=data/bpic_2012/data/original/bpi12_knobab.xes --maxsat=scripts/maxsat_pipeline.yaml --declareFile="$model" --csv=data/results/phi.csv --queryCount=5
	done
done

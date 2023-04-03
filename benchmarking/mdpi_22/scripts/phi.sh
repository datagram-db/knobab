#!/bin/bash
cd ../../..

declare -a models=("benchmarking/mdpi_22/data/bpic_2012/models/5.powerdecl" "benchmarking/mdpi_22/data/bpic_2012/models/10.powerdecl" "benchmarking/mdpi_22/data/bpic_2012/models/15.powerdecl" "benchmarking/mdpi_22/data/bpic_2012/models/20.powerdecl")
declare -a pipelines=("scripts/atomization_pipeline.yaml" "scripts/atomization_always_pipeline.yaml")

for model in "${models[@]}"; do
	for pipeline in "${pipelines[@]}"; do
		./cmake-build-release/knobab-v1.0 --nostats --atomization="$pipeline"  --xes=benchmarking/mdpi_22/data/bpic_2012/logs/original/bpi12_knobab.xes --maxsat=scripts/maxsat_pipeline.yaml --declareFile="$model" --csv=benchmarking/mdpi_22/data/results/phi.csv --queryCount=5
	done
done

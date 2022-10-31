#!/bin/bash
cd ..
cd ..
cd ..

declare -a models=("data/testing/bpic_2012/models/scenario_1/5.powerdecl" "data/testing/bpic_2012/models/scenario_1/10.powerdecl" "data/testing/bpic_2012/models/scenario_1/15.powerdecl" "data/testing/bpic_2012/models/scenario_1/20.powerdecl" "data/testing/bpic_2012/models/scenario_2/q1.powerdecl" "data/testing/bpic_2012/models/scenario_2/scenario_2/q2.powerdecl" "data/testing/bpic_2012/models/scenario_2/q3.powerdecl" "data/testing/bpic_2012/models/scenario_2/q4.powerdecl" "data/testing/bpic_2012/models/scenario_2/q5.powerdecl" "data/testing/bpic_2012/models/scenario_2/q1^q2.powerdecl" "data/testing/bpic_2012/models/scenario_2/q1^q2^q3^q4^q5.powerdecl" "data/testing/bpic_2012/models/scenario_2/q1^q2^q4.powerdecl" "data/testing/bpic_2012/models/scenario_2/q1^q2^q5.powerdecl" "data/testing/bpic_2012/models/scenario_2/q1+q3+q4.powerdecl" "data/testing/bpic_2012/models/scenario_2/q1^q3^q5.powerdecl")

declare -a pipelines=("scripts/intersection_pipeline.yaml" "scripts/support_pipeline.yaml")

for model in "${models[@]}"; do
	for pipeline in "${pipelines[@]}"; do
		./cmake-build-release/knobab-v1.0 --nostats --xes=data/testing/bpic_2012/data/original/bpi12_knobab.xes --maxsat="$pipeline" --declareFile="$model" --csv=data/testing/results/benchmarking/burattin.csv --queryCount=5
	done
done

#!/usr/bin/env bash
cd ..
cd ..

# Declare a string array with type
declare -a StringArray=("AltPrecedence" "AltResponse" "ChainPrecedence" "ChainResponse" "NegSuccession" "Precedence" "RespExistence" "Response")
declare -a ProcArray=("scripts/maxsat_pipeline2.yaml" "scripts/maxsat_pipeline4.yaml" "scripts/maxsat_pipeline6.yaml" "scripts/maxsat_pipeline8.yaml")
#declare -a plist=(2 4 8 16)
declare -a plist=(15)
declare -a schedulesArray=("BLOCK_STATIC_SCHEDULE" "CYCLIC_STATIC_SCHEDULE" "GUIDED_SCHEDULE" "MONOTONIC_DYNAMIC_SCHEDULE" "TASK_SIZE_PREDICTION_BLOCK_STATIC" "TASK_SIZE_PREDICTION_UNBALANCED_DYNAMIC")

# Read the array values with space
for ms in "${plist[@]}"; do
	for val in "${StringArray[@]}"; do
		./parall/knobab-v1.0 --nostats --nodata --queryCount 5 --topN $ms --topNTemplate=$val --xes=data/bpic_2011/data/original/log.xes --maxsat=scripts/maxsat_pipeline.yaml --csv=data/results/parallel.csv
		for proc in "${ProcArray[@]}"; do
		  	for sched in "${schedulesArray[@]}"; do
#			      echo "$ms $val $proc $sched"
#			      sleep 3
			      ./parall/knobab-v1.0 --nostats --nodata --queryCount 5 --topN $ms --topNTemplate=$val --xes=data/bpic_2011/data/original/log.xes --maxsat=$proc --csv=data/results/parallel.csv --scheduler=$sched
			done
		done
	done
done




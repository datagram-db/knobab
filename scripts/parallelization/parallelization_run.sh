#!/usr/bin/env bash
# Declare a string array with type
declare -a StringArray=("AltPrecedence" "AltResponse" "ChainPrecedence" "ChainResponse" "NegSuccession" "Precedence" "RespExistence" "Response")
declare -a ProcArray=("scripts/maxsat_pipeline2.yaml" "scripts/maxsat_pipeline4.yaml" "scripts/maxsat_pipeline6.yaml" "scripts/maxsat_pipeline8.yaml")
#declare -a plist=(2 4 8 16)
declare -a plist=(16)
declare -a SchedArray=("BLOCK_STATIC_SCHEDULE" "CYCLIC_STATIC_SCHEDULE" "GUIDED_SCHEDULE" "MONOTONIC_DYNAMIC_SCHEDULE" "TASK_SIZE_PREDICTION_BLOCK_STATIC" "TASK_SIZE_PREDICTION_UNBALANCED_DYNAMIC")
 
# Read the array values with space
for ms in "${SchedArray[@]}"; do
	for val in "${StringArray[@]}"; do
		./fast_build/server -n -o -q 5 -p $ms --topNTemplate=$val --xes=/home/giacomo/projects/knobab/data/testing/hospital/standard/Hospital_Log.xes  --maxsat=scripts/maxsat_pipeline.yaml --csv=parall.csv
		for proc in "${ProcArray[@]}"; do
			echo "$ms $val $proc"
			sleep 3
			./parall/server -n -o -q 5 -p $ms --topNTemplate=$val --xes=/home/giacomo/projects/knobab/data/testing/hospital/standard/Hospital_Log.xes  --maxsat=$proc --csv=parall.csv
		done
	done
done




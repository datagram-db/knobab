#!/usr/bin/env bash
# Declare a string array with type
declare -a StringArray=("AltPrecedence" "AltResponse" "ChainPrecedence" "ChainResponse" "NegSuccession" "Precedence" "RespExistence" "Response")
declare -a ProcArray=("scripts/maxsat_pipeline2.yaml" "scripts/maxsat_pipeline4.yaml" "scripts/maxsat_pipeline6.yaml" "scripts/maxsat_pipeline8.yaml")
#declare -a plist=(2 4 8 16)
declare -a plist=(16)
declare -a SchedArray=("BLOCK_STATIC_SCHEDULE" "CYCLIC_STATIC_SCHEDULE" "GUIDED_SCHEDULE" "MONOTONIC_DYNAMIC_SCHEDULE" "TASK_SIZE_PREDICTION_BLOCK_STATIC" "TASK_SIZE_PREDICTION_UNBALANCED_DYNAMIC")
 
# Read the array values with space

for val in "${StringArray[@]}"; do
	./cmake-build-release/knobab-v1.0 -n -o -q 5 -e $ms --topN=5 --topNTemplate=$val --xes=/home/giacomo/projects/knobab/data/testing/bpic_2011/data/original/log.xes  --maxsat=scripts/maxsat_pipeline.yaml --csv=parall.csv
	if [ $? -neq 0 ]; then
	    echo "It crashed!"
	    exit 1
	fi
	for proc in "${ProcArray[@]}"; do
		for ms in "${SchedArray[@]}"; do
			echo "$ms $val $proc"
			./parall/knobab-v1.0 -n -o -q 5 -e $ms --topN=5 --topNTemplate=$val --xes=/home/giacomo/projects/knobab/data/testing/bpic_2011/data/original/log.xes  --maxsat=$proc --csv=parall.csv
			if [ $? -neq 0 ]; then
	    			echo "It crashed!"
	    			exit 1
			fi
		done
	done
done




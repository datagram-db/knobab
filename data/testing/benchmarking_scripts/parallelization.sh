#!/usr/bin/env bash
cd ..
cd ..
cd ..

# Declare a string array with type
declare -a StringArray=("AltPrecedence" "AltResponse" "ChainPrecedence" "ChainResponse" "NegSuccession" "Precedence" "RespExistence" "Response")
declare -a ProcArray=("scripts/maxsat_pipeline.yaml" "scripts/maxsat_pipeline2.yaml" "scripts/maxsat_pipeline4.yaml" "scripts/maxsat_pipeline6.yaml" "scripts/maxsat_pipeline8.yaml")
#declare -a plist=(2 4 8 16)
declare -a plist=(16)
 
 
# Read the array values with space
for ms in "${plist[@]}"; do
	for val in "${StringArray[@]}"; do
		for proc in "${ProcArray[@]}"; do
			echo "$ms $val $proc"
			#sleep 3
			./cmake-build-release/knobab-v1.0 -n -o -q 5 -p $ms --topNTemplate=$val --topN=15 --xes=data/testing/bpic_2011/data/original/log.xes  --maxsat=$proc --csv=data/testing/results/benchmarking/parallel.csv --queryCount=5
		done
	done
done




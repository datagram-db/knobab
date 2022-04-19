#!/usr/bin/env bash
# Declare a string array with type
declare -a StringArray=("AltPrecedence" "AltResponse" "ChainPrecedence" "ChainResponse" "NegSuccession" "Precedence" "RespExistence" "Response")
declare -a plist=("data/testing/hospital/10/Hospital_Log_10.xes" "data/testing/hospital/100/Hospital_Log_100.xes" "data/testing/hospital/1000/Hospital_Log_1000.xes")
 
# Read the array values with space
for ms in "${plist[@]}"; do
	for val in "${StringArray[@]}"; do
		./fast_build/server -n -o -p 5 -q 5  --topNTemplate=$val --xes=$ms  --maxsat=scripts/maxsat_pipeline.yaml --csv=mining_run.csv
		./fast_build/server -n -o -p 5 -q 5  --topNTemplate=$val --xes=$ms  --maxsat=scripts/support_pipeline.yaml --csv=mining_run.csv
	done
done




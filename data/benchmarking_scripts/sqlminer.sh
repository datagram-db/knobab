#!/bin/bash
cd ..
cd ..

declare -a clauses=("Response" "AltResponse" "ChainResponse" "Precedence" "AltPrecedence" "ChainPrecedence" "RespExistence" "NegSuccession")
declare -a samples=("data/bpic_2011/data/10/log.xes" "data/bpic_2011/data/100/log.xes" "data/bpic_2011/data/1000/log.xes")
declare -a pipeline=("scripts/maxsat_pipeline.yaml" "scripts/maxsat_pipeline_hybrid.yaml" "scripts/support_pipeline.yaml" "scripts/support_pipeline_hybrid.yaml")

for clause in "${clauses[@]}"; do
	for sample in "${samples[@]}"; do
		for p in "${pipeline[@]}"; do
			./cmake-build-release/knobab-v1.0 --xes="$sample" --maxsat="$p" --topNTemplate="$clause" --topN=5 --csv=data/results/sql.csv --queryCount=5
		done
	done
done

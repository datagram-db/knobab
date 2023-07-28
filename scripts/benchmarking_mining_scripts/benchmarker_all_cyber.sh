set -x #echo on
algorithm=("bolt2" "apriori")
logs=("data/benchmarking/mining/cyber_all/all.tab_9.tab"
"data/benchmarking/mining/cyber_all/all.tab_81.tab"
"data/benchmarking/mining/cyber_all/all.tab_729.tab"
"data/benchmarking/mining/cyber_all/all.tab_6552.tab")

for a in "${algorithm[@]}"; do
	for l in "${logs[@]}"; do
		./cmake-build-release/mining -s 0.9 -a "$a" -t "$l" -o "data/benchmarking/mining/results_cyber_all.csv" -q 5
	done
done

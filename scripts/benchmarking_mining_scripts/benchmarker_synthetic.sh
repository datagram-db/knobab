set -x #echo on
algorithm=("bolt2" "apriori" "topk")
logs=("data/benchmarking/mining/synthetic/logs/10_10_10.xes" "data/benchmarking/mining/synthetic/logs/10_15_10.xes"
      "data/benchmarking/mining/synthetic/logs/10_20_10.xes" "data/benchmarking/mining/synthetic/logs/10_25_10.xes"
      "data/benchmarking/mining/synthetic/logs/10_30_10.xes" "data/benchmarking/mining/synthetic/logs/100_10_10.xes"
      "data/benchmarking/mining/synthetic/logs/100_15_10.xes" "data/benchmarking/mining/synthetic/logs/100_20_10.xes"
      "data/benchmarking/mining/synthetic/logs/100_25_10.xes" "data/benchmarking/mining/synthetic/logs/100_30_10.xes"
      "data/benchmarking/mining/synthetic/logs/1000_10_10.xes" "data/benchmarking/mining/synthetic/logs/1000_15_10.xes"
      "data/benchmarking/mining/synthetic/logs/1000_20_10.xes" "data/benchmarking/mining/synthetic/logs/1000_25_10.xes"
      "data/benchmarking/mining/synthetic/logs/1000_30_10.xes")

for a in "${algorithm[@]}"; do
	for l in "${logs[@]}"; do
		./cmake-build-release/mining -s 0.9 -a "$a" -x "$l" -o "data/benchmarking/mining/results_synthetic.csv" -q 5
	done
done

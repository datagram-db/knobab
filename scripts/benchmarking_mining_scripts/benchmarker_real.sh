cd ..

support=(0.1 0.25 0.5 0.9)
algorithm=("b" "a" "p")
logs=("data/benchmarking/mining/bpic_2019/logs/10.xes" "data/benchmarking/mining/bpic_2019/logs/100.xes" "data/benchmarking/mining/bpic_2019/logs/1000.xes" "data/benchmarking/mining/bpic_2019/logs/10000.xes" "data/benchmarking/mining/bpic_2019/logs/100000.xes")

for s in "${support[@]}"; do
	for a in "${algorithm[@]}"; do
		for l in "${logs[@]}"; do
			./cmake-build-release/mining -s $s -"$a" -x "$l" -o "data/benchmarking/mining/results_real.csv" -q 5
		done
	done
done

set -x #echo on
support=(0.1 0.25 0.5 0.9)
algorithm=("bolt2")
logs=("data/benchmarking/mining/cyber_all/all.tab_9.tab"
"data/benchmarking/mining/cyber_all/all.tab_81.tab"
"data/benchmarking/mining/cyber_all/all.tab_729.tab"
"data/benchmarking/mining/cyber_all/all.tab_6552.tab")

for l in "${logs[@]}"; do
  for s in "${support[@]}"; do
    for a in "${algorithm[@]}"; do
      echo "s: $s t: $l"
      ./cmake-build-release/mining -s "$s"  -a "$a" -t "$l" -o "data/benchmarking/mining/results_cyber_all2.csv" -q 5
    done
  done
done

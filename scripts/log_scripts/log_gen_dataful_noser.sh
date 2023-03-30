lens=(10 100 1000 10000)
logs=("/home/giacomo/projects/loggen/convert_same/data/test_gen_2_dataful.powerdecl" "/home/giacomo/projects/loggen/convert_same/data/test_gen_4_dataful.powerdecl" "/home/giacomo/projects/loggen/convert_same/data/test_gen_6_dataful.powerdecl" "/home/giacomo/projects/loggen/convert_same/data/test_gen_8_dataful.powerdecl" "/home/giacomo/projects/loggen/convert_same/data/test_gen_10_dataful.powerdecl")

echo "dataful Testing"
for logS in "${lens[@]}"; do
	for traceS in "${lens[@]}"; do
		for l in "${logs[@]}"; do
			/home/giacomo/projects/knobab2/cmake-build-release/log_generator "/home/giacomo/projects/knobab2/data/cache/graph_pattern_cache2" "$l" "/home/giacomo/projects/loggen/convert_same/data/test_gen_10_knobab_sigma.txt" "results.csv" -m $traceS -M $traceS -L $logS -N
			/home/giacomo/projects/knobab2/cmake-build-release/log_generator "/home/giacomo/projects/knobab2/data/cache/graph_pattern_cache2" "$l" "/home/giacomo/projects/loggen/convert_same/data/test_gen_10_knobab_sigma.txt" "results.csv" -m $traceS -M $traceS -L $logS -N
			/home/giacomo/projects/knobab2/cmake-build-release/log_generator "/home/giacomo/projects/knobab2/data/cache/graph_pattern_cache2" "$l" "/home/giacomo/projects/loggen/convert_same/data/test_gen_10_knobab_sigma.txt" "results.csv" -m $traceS -M $traceS -L $logS -N
			/home/giacomo/projects/knobab2/cmake-build-release/log_generator "/home/giacomo/projects/knobab2/data/cache/graph_pattern_cache2" "$l" "/home/giacomo/projects/loggen/convert_same/data/test_gen_10_knobab_sigma.txt" "results.csv" -m $traceS -M $traceS -L $logS -N
			/home/giacomo/projects/knobab2/cmake-build-release/log_generator "/home/giacomo/projects/knobab2/data/cache/graph_pattern_cache2" "$l" "/home/giacomo/projects/loggen/convert_same/data/test_gen_10_knobab_sigma.txt" "results.csv" -m $traceS -M $traceS -L $logS -N
		done
	done
done

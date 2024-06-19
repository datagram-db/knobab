find /home/giacomo/projects/knobab2_loggen/output_model_healthcare/debugged/ -maxdepth 1 -mindepth 1 -type d |while read fname; do
  ./cmake-build-release/knobab_json -f "$fname" -s 0.0 -d user -i day -i span -i "__class" -i "__label" -i time -i fulltime -p /home/giacomo/projects/sdd-processing/sdd-processing/log_weekly.json;
done
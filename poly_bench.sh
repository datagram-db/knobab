#!/usr/bin/env bash
for i in {2..10..2} ; do
  x=`echo "scale=2 ; ${i}/10" | bc`
  ./cmake-build-release/knobab_json -s $x -d user -i day -i span -i "__class" -i "__label" -i time -i fulltime -r -p /home/giacomo/projects/sdd-processing/sdd-processing/log_weekly.json
  ./cmake-build-release/knobab_json -s $x -d user -i day -i span -i "__class" -i "__label" -i time -i fulltime -p /home/giacomo/projects/sdd-processing/sdd-processing/log_weekly.json
  ./cmake-build-release/knobab_json -s $x -d user -i day -i span -i "__class" -i "__label" -i time -i fulltime -r -l -p  /home/giacomo/projects/sdd-processing/sdd-processing/log_weekly.json
  ./cmake-build-release/knobab_json -s $x -d user -i day -i span -i "__class" -i "__label" -i time -i fulltime -l -p /home/giacomo/projects/sdd-processing/sdd-processing/log_weekly.json
done
./cmake-build-release/knobab_json -s 0.9 -d user -i day -i span -i "__class" -i "__label" -i time -i fulltime -r -p /home/giacomo/projects/sdd-processing/sdd-processing/log_weekly.json
./cmake-build-release/knobab_json -s 0.9 -d user -i day -i span -i "__class" -i "__label" -i time -i fulltime -p /home/giacomo/projects/sdd-processing/sdd-processing/log_weekly.json
./cmake-build-release/knobab_json -s 0.9 -d user -i day -i span -i "__class" -i "__label" -i time -i fulltime -r -l -p  /home/giacomo/projects/sdd-processing/sdd-processing/log_weekly.json
./cmake-build-release/knobab_json -s 0.9 -d user -i day -i span -i "__class" -i "__label" -i time -i fulltime -l -p /home/giacomo/projects/sdd-processing/sdd-processing/log_weekly.json
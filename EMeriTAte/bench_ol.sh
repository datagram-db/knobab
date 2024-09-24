declare -a arr=("True" "False")
export patho="/home/giacomo/projects/knobab2_loggen/EMeriTAte/osuleaf/"
echo "Concerning ${patho}" >> errors.log
## now loop through the above array
for i in "${arr[@]}"
do
   for j in "${arr[@]}"
   do
      for s in {0,0.2,0.4,0.6,0.8,1}
      do
        echo "~~~~~ Running ${s} ${i} ${j} ~~~~~" >> errors.log
        /home/giacomo/projects/knobab2_loggen/EMeriTAte/venv/bin/python /home/giacomo/projects/knobab2_loggen/EMeriTAte/other_processing_steps.py $patho "class" 6 "time"  $s $i $j 2>> errors.log
        if [ $? -eq 0 ]
        then
          echo "Successfully created file"
        else
          echo "See Above: Could not mine tests for ${s} ${i} ${j}" >> errors.log
        fi
        echo "~~~~~ Done ${s} ${i} ${j} ~~~~~" >> errors.log
      done
   # or do whatever with individual element of the array
   done
done
#/home/giacomo/projects/knobab2_loggen/EMeriTAte/venv/bin/python /home/giacomo/projects/knobab2_loggen/EMeriTAte/other_processing_steps.py $patho "class" "time"
#if [ $? -eq 0 ]
#        then
#          echo "Successfully finaliesd the run created file"
#        else
#          echo "Could not finalised"
#fi



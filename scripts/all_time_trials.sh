#!/bin/bash
rm results.txt
for i in 20 # 1 5 10 20
do
  for point in 1000 # 100 200 300 400 500 600 700 800 900 1000
  do
    for center in 32 64 # 2 4 8 16 32 64
    do
      for processes in 1 2 4 8
      do
        if [ $center -ge $processes ] ; then
          points=${point},${point}
          centers=random_centers/${center}_in_${point}-${point}.txt
          echo i=${i} points=$points centers=${centers} processes=${processes} >> results.txt
          ./time_trials.sh $i $points $centers $processes | tail -n1 >> results.txt
          echo "" >> results.txt
        fi
      done
    done
  done
done
#!/bin/bash
output=results/test
iterations=100
points=1125,2436
distance=manhattan
centers=50

#for color in red blue green yellow orange purple black white
#do
#  python color_generator.py --color $color --x $centers --n 40
#done

for k in 1 2 3 # 0
do
  ./cmake-build-debug/voronoi_generation -d $distance -b $points -i $iterations -c $centers -o $output -r -m 1 -k $k

  for color in red blue green yellow orange purple black white
  do
      save_img=background/k[${k}]_c[${color}].gif
      python display_results.py --mode cells --results_dirc $output --colors colors/${color}.txt --save_path $save_img
  done
done


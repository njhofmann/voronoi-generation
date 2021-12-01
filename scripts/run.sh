#!/bin/bash

output=results/test
dist=manhattan
p=1 # param for some distance functions
i=100 # iterations
m=4  # processes
k=1 # order
points=1125,2436
c=50 # centers random_centers/8_in_500-500.txt
mode=cells # centers, cells
save_img=imgs/${dist}_${k}.gif
./cmake-build-debug/voronoi_generation -d $dist -b $points -i $i -c $c -o $output -r -p $p -k $k  #-f
echo finished generating points
python display_results.py --mode $mode --results_dirc $output --colors examples/oranges.txt --save_path $save_img
#!/bin/bash

output=results/test
dist=euclidean
p=1 # param for some distance functions
i=100 # iterations
m=4  # processes
k=0 # order
points=1000,1000
c=50 # centers random_centers/8_in_500-500.txt
mode=centers # centers, cells
save_img=imgs/${dist}_${k}.gif
./cmake-build-debug/voronoi_generation -d $dist -b $points -i $i -c 30 -o $output -f -r -p $p -m $m -k $k
echo finished generating points
python display_results.py --mode $mode --point_centers ${output}/point_centers.txt --centers ${output}/centers.txt --colors examples/colors.txt --save_path $save_img
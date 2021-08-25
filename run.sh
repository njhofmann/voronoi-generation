#!/bin/bash

output=results/test
dist=manhattan
p=1
i=20
m=4
k=-1
points=1125,2436
c=40
./cmake-build-debug/voronoi_generation -d $dist -b $points -c $c -i $i -o $output -r -p $p -m $m -k $k
echo finished generating points
python display_results.py ${output}/point_centers.txt ${output}/centers.txt examples/colors.txt imgs/large.gif
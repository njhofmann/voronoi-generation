#!/bin/bash

output=results/test
dist=chebyshev
p=1
i=200
m=4
points=1000,1000
./cmake-build-debug/voronoi_generation -d $dist -b $points -c random_centers/64_in_1000-1000.txt -i $i -f -o $output -r -p $p -m $m
echo finished generating points
python display_results.py ${output}/point_centers.txt ${output}/centers.txt examples/colors.txt imgs/large.gif
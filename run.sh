#!/bin/bash

output=results/test
dist=bray-curtis
p=1
i=20
m=4
./cmake-build-debug/voronoi_generation -d $dist -b 500,500 -c examples/centers.txt -i $i -f -o $output -r -p $p -m $m
echo finished generating points
python display_results.py ${output}/point_centers.txt ${output}/centers.txt examples/colors.txt imgs/large.gif
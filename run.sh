#!/bin/bash

output=results/test
dist=euclidean
p=1
./cmake-build-debug/voronoi_generation -d $dist -b 0,0 300,300 -c examples/med_centers.txt -i 1 -f -o $output -r -p $p -m 1
echo finished generating points
python display_results.py ${output}/point_centers.txt ${output}/centers.txt examples/colors.txt imgs/large.gif
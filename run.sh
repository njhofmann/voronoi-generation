#!/bin/bash

output=results/test
dist=euclidean
p=2
./cmake-build-debug/voronoi_generation -d $dist -b 0,0 500,500 -c examples/centers.txt -i 10 -f -o $output -r -p $p -m 2
echo finished generating points
python display_results.py ${output}/point_centers.txt ${output}/centers.txt examples/colors.txt imgs/large.gif
#!/bin/bash

output=results/test
dist=euclidean
p=1
./cmake-build-debug/voronoi_generation -d $dist -b 200,300 -c examples/small_centers.txt -i 10 -f -o $output -r -p $p -m 1
echo finished generating points
python display_results.py ${output}/point_centers.txt ${output}/centers.txt examples/colors.txt imgs/large.gif
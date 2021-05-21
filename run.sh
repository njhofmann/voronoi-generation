#!/bin/bash

output=results/test
dist=yang
p=3
./cmake-build-debug/voronoi_generation -d $dist -b 0,0 414,896 -c examples/large_centers.txt -i 10 -f -o $output -r -p $p
python display_results.py ${output}/point_centers.txt ${output}/centers.txt examples/colors.txt imgs/large.gif
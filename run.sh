#!/bin/bash

output=results/test
dist=yang
./cmake-build-debug/voronoi_generation -d $dist -b 0,0 500,500 -c examples/centers.txt -i 1000 -o $output -r -p 3
python display_results.py ${output}/point_centers.txt ${output}/centers.txt examples/colors.txt imgs/test.gif
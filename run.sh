#!/bin/bash

output=results/test.txt
dist=minkowski-4
./cmake-build-debug/voronoi_generation -d $dist -b 0,0 500,500 -c examples/centers.txt -i 60 -o $output -r -f
python display_results.py $output $dist
#!/bin/bash

output=results/test.txt
./cmake-build-debug/voronoi_generation -b 0,0 500,500 -c examples/centers.txt -v 2 -o $output -r -f
python display_results.py $output
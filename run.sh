#!/bin/bash

output=results/test.txt
./cmake-build-debug/voronoi_generation -d mse -b 0,0 500,500 -c examples/centers.txt -i 50 -o $output -r -f
python display_results.py $output test
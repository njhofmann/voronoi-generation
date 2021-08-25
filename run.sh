#!/bin/bash

output=results/test
dist=euclidean
p=1
i=1
m=4
k=-1
points=500,500
c=random_centers/8_in_500-500.txt
save_img=imgs/${dist}_${k}.gif
./cmake-build-debug/voronoi_generation -d $dist -b $points -c $c -i $i -o $output -f -r -p $p -m $m -k $k
echo finished generating points
python display_results.py ${output}/point_centers.txt ${output}/centers.txt examples/colors.txt $save_img
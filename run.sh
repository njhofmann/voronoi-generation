#!/bin/bash

output=results/test
dist=euclidean
p=1
i=20
m=4
k=0
points=20,20,20
c=random_centers/8_in_500-500.txt
save_img=imgs/${dist}_${k}.jpg
./cmake-build-debug/voronoi_generation -d $dist -b $points -i $i -c 10 -o $output -f -r -p $p -m $m -k $k
echo finished generating points
python display_results.py --point_centers ${output}/point_centers.txt --centers ${output}/centers.txt --colors examples/colors.txt --save_path $save_img
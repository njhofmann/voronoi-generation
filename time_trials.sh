#!/bin/bash
trials=10
i=10
processes=2
points=1500,1500
rm -f /tmp/mtime.$$
for (( j = 0; j < $trials; j++ ));
do
  /usr/bin/time -f "real %e user %U sys %S" -a -o /tmp/mtime.$$ $@ ./cmake-build-debug/voronoi_generation -d euclidean -b 0,0 $points -c examples/small_centers.txt -i $i -f -o test/${j} -r -p 2 -m $processes
  tail -1 /tmp/mtime.$$
done

awk '{ et += $2; ut += $4; st += $6; count++ } END {  printf "Average: real %.3f user %.3f sys %.3f\n", et/count, ut/count, st/count }' /tmp/mtime.$$
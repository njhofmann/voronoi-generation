#!/bin/bash
trials=10
i=$1  # 1, 5, 10, 20
points=$2
centers=$3  # 2, 4, 8, 16, 32, 64
processes=$4
rm -f /tmp/mtime.$$
for (( j = 0; j < $trials; j++ ));
do
  echo i=${i} points=$points centers=${centers} processes=${processes} >&2
  /usr/bin/time -f "real %e user %U sys %S" -a -o /tmp/mtime.$$ ./cmake-build-debug/voronoi_generation -d euclidean -b $points -c $centers -i $i -f -o test/${j} -r -p 2 -m $processes
  tail -1 /tmp/mtime.$$
done

awk '{ et += $2; ut += $4; st += $6; count++ } END {  printf "avg: real %.4f user %.4f sys %.4f\n", et/count, ut/count, st/count }' /tmp/mtime.$$
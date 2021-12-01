#!/bin/bash
output=results/test
iterations=100
points=500,500
# TODO p and other dists
# TODO
for distance in euclidean manhattan
do
  for centers in 5 7 8 9 10 20 30
  do
    for k in 0 #1 2 3
    do
      if [ $k -eq 0 ]; then
        ./cmake-build-debug/voronoi_generation -d $distance -b $points -i $iterations -c $centers -o $output -f -r -k $k
      else
        ./cmake-build-debug/voronoi_generation -d $distance -b $points -i $iterations -c $centers -o $output -r -m 1 -k $k
      fi

      echo finished generating points

      for mode in cells centers
      do
        save_img=imgs/d[${distance}]_c[${centers}]_k[${k}]_p[${points}]_m[${mode}]_i[${iterations}].gif
        python display_results.py --mode $mode --results_dirc $output --colors examples/colors.txt --save_path $save_img --reversed
      done
    done
  done
done


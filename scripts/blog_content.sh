#!/bin/bash

output=../results/test
iters=100 # iterations
seed=4

create_diagram () {
  # metric = $1
  # p = $2
  # k = $3
  # d = $4

  if [ $4 == 2 ]; then
    n_centers=32 # number of centers
    points=${size},${size}
    centers=../random_centers/${n_centers}_in_${size}-${size}.txt
  elif [ $4 == 3 ]; then
    n_centers=32
    size=100 # side length of box
    points=${size},${size},${size}
    centers=../random_centers/${n_centers}_in_${size}-${size}-${size}.txt
  fi

  if [[ $3 == 0 ]]; then
    ../cmake-build-debug/voronoi_generation -d $dist -b $points -i $iters -c $centers -o $output -r -s $seed -p $2 -k $3 -f
  else
    ../cmake-build-debug/voronoi_generation -d $dist -b $points -i $iters -c $centers -o $output -r -s $seed -p $2 -k $3
  fi

  if [[ $3 != 0 || $4 == 3 ]]; then
    file_type=jpg
  else
    file_type=gif
  fi

  for mode in cells centers
  do
    save_img=../imgs/d[${1}]_c[${n_centers}]_k[${3}]_pts[${points}]_m[${mode}]_i[${iters}]_p[${2}].${file_type}
    python ../display_results.py --mode $mode --results_dirc $output --colors ../examples/colors.txt --save_path $save_img
  done
}

for dim in 3 #2 3
do
  for k in 0 # 1 -1
  do
    for dist in manhattan yang minkowski #hellinger yang minkowski euclidean bray-curtis manhattan chebyshev canberra
    do
      if [[ "$dist" == "yang" || "$dist" == "minkowski" ]]; then
        for p in 3 4 5
        do
          create_diagram $dist $p $k $dim
        done
      else
        create_diagram $dist 1 $k $dim
      fi
    done
  done
done
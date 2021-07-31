//
// Created by natejh on 1/6/21.
//

#ifndef VORONOI_GENERATION__VORONOI_H_
#define VORONOI_GENERATION__VORONOI_H_

#include <stdio.h>
#include "int_matrix.h"
#include "distance_metric.h"

void voronoi_relaxation(IntArray* dimensions, IntMatrix* points, IntMatrix* centers, DistanceMetric metric,
                        int iterations, double converge_threshold, char* output_dirc, bool full_output,
                        int process_cnt, int p);
#endif //VORONOI_GENERATION__VORONOI_H_

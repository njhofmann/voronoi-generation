//
// Created by natejh on 1/6/21.
//

#ifndef VORONOI_GENERATION__VORONOI_H_
#define VORONOI_GENERATION__VORONOI_H_

#include <stdio.h>
#include "int_matrix.h"
#include "distance_metric.h"

typedef struct {
  IntArray* center;
  IntMatrix* points;
} Cell;

typedef struct {
  Cell** cells;
  int size;
} Cells;

void voronoi_relaxation(IntMatrix* points, IntMatrix* centers, DistanceMetric metric, int iterations,
                        double converge_threshold, FILE* stream, bool full_output);
#endif //VORONOI_GENERATION__VORONOI_H_

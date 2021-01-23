//
// Created by natejh on 1/6/21.
//

#ifndef VORONOI_GENERATION__VORONOI_H_
#define VORONOI_GENERATION__VORONOI_H_

#include "int_matrix.h"
#include "distance_metric.h"

typedef struct {
  IntArray* center;
  IntMatrix* points;
} Cell;

Cell** voronoi_diagram(IntMatrix* centers, IntMatrix* points, DistanceMetric metric);

#endif //VORONOI_GENERATION__VORONOI_H_

//
// Created by natejh on 1/6/21.
//

#include "int_array.h"
#ifndef VORONOI_GENERATION__DISTANCE_METRIC_H_
#define VORONOI_GENERATION__DISTANCE_METRIC_H_

typedef enum {
  EUCLIDEAN = 1,
  MANHATTAN = 2,
  MAHALANOBIS = 3,
  RAMANUJAN = 4
} DistanceMetric;

DistanceMetric parse_distance_metric(char* raw_arg);
double compute_distance_metric(IntArray* a, IntArray* b, DistanceMetric metric);

#endif //VORONOI_GENERATION__DISTANCE_METRIC_H_

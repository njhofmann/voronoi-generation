//
// Created by natejh on 1/6/21.
//

#include "int_array.h"
#ifndef VORONOI_GENERATION__DISTANCE_METRIC_H_
#define VORONOI_GENERATION__DISTANCE_METRIC_H_

/**
 * Available distance metrics
 */
typedef enum {
  EUCLIDEAN = 0,
  MANHATTAN = 1,
  RAMANUJAN = 2,
  CHEBYSHEV = 3,
  CANBERRA = 4,  // weighted manhattan
  PEARSON = 5,  // 1 - correlation
  COSINE = 6,
  BRAY_CURTIS = 7,
  MINKOWSKI = 8,
  YANG = 9,
} DistanceMetric;

DistanceMetric parse_distance_metric(char* raw_arg);
double compute_distance_metric(IntArray* a, IntArray* b, DistanceMetric metric, int p);

#endif //VORONOI_GENERATION__DISTANCE_METRIC_H_

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
  EUCLIDEAN = 1,
  MANHATTAN = 2,
  RAMANUJAN = 3,
  CHEBYSHEV = 4,
  CANBERRA = 5,  // weighted manhattan
  PEARSON = 6,  // 1 - correlation
  COSINE = 7,
  BRAY_CURTIS = 8,
  MINKOWSKI = 9,
  YANG = 10,
} DistanceMetric;

DistanceMetric parse_distance_metric(char* raw_arg);
double compute_distance_metric(IntArray* a, IntArray* b, DistanceMetric metric, int p);

#endif //VORONOI_GENERATION__DISTANCE_METRIC_H_

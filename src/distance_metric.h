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
  SSD = 3,  // sum of squared difference
  RAMANUJAN = 4,
  CHEBYSHEV = 5,
  CANBERRA = 6,  // weighted manhattan
  PEARSON = 7,  // 1 - correlation
  COSINE = 8,
  BRAY_CURTIS = 9,
  MINKOWSKI_3 = 10,
  MINKOWSKI_4 = 11,
  YANG_2 = 12,
  YANG_3 = 13,
  YANG_4 = 14
} DistanceMetric;

DistanceMetric parse_distance_metric(char* raw_arg);
double compute_distance_metric(IntArray* a, IntArray* b, DistanceMetric metric);

#endif //VORONOI_GENERATION__DISTANCE_METRIC_H_

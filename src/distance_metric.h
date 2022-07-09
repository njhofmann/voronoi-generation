//
// Created by natejh on 1/6/21.
//

#include "int_array.h"
#ifndef VORONOI_GENERATION__DISTANCE_METRIC_H_
#define VORONOI_GENERATION__DISTANCE_METRIC_H_

/**
 * Supported distance metrics
 */
typedef enum {
  EUCLIDEAN = 0,
  MANHATTAN = 1,
  CHEBYSHEV = 2,
  CANBERRA = 3,
  BRAY_CURTIS = 4,
  MINKOWSKI = 5,
  YANG = 6,
  HELLINGER = 7,
} DistanceMetric;


/**
 * Returns the distance metric corresponding to the given string, throws an error if invalid string
 */
DistanceMetric parse_distance_metric(char* raw_arg);

/**
 * Computes the distance between two given IntArrays using the given distance metric, assumes arrays are of the same
 * length
 */
double compute_distance_metric(IntArray* a, IntArray* b, DistanceMetric metric, int p);

#endif //VORONOI_GENERATION__DISTANCE_METRIC_H_

//
// Created by natejh on 8/24/21.
//

#include "int_array.h"
#include "int_matrix.h"
#include "distance_metric.h"
#ifndef VORONOI_GENERATION_SRC_QUICKSELECT_H_
#define VORONOI_GENERATION_SRC_QUICKSELECT_H_

/**
 * Finds the index of the `k`-th closest center to the given point via the given distance metric. Assumes the point
 * and centers are of the same dimensionality
 */
int k_th_closest_center(IntArray* point, IntMatrix* centers, DistanceMetric distance_metric, int p, int k);

#endif //VORONOI_GENERATION_SRC_QUICKSELECT_H_

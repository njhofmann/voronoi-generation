//
// Created by natejh on 5/14/21.
//

#ifndef VORONOI_GENERATION_SRC_POINTS_H_
#define VORONOI_GENERATION_SRC_POINTS_H_

#include "int_matrix.h"

/**
 * Returns an IntMatrix of all the points in the given bounding box
 *
 * Ex: if bounding box if from (0,0) to (10, 10) - returns (0, 0), (0, 1), ..., (9, 8), (9, 9)
 */
IntMatrix* get_points_in_bounding_box(IntArray* box);

/**
 * Returns if the given set of centers are within the given bounding box
 */
void valid_centers(IntArray* box, IntMatrix* centers);

/**
 * Returns the number of points in the given bounding box
 */
int num_of_points(IntArray* box);

/**
 * Returns `count` random points within the given bounds
 */
IntMatrix* random_points(int count, IntArray* bounds);

#endif //VORONOI_GENERATION_SRC_POINTS_H_

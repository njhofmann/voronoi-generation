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
IntMatrix* get_points_in_bounding_box(IntMatrix* box);

/**
 * Returns the dimensions of the given bounding box, i.e. how big is each axis
 */
IntArray* get_bounding_box_dims(IntMatrix* box);

/**
 * Returns if the given set of centers are within the given bounding box
 * @param box
 * @param centers
 */
void valid_centers(IntMatrix* box, IntMatrix* centers);

#endif //VORONOI_GENERATION_SRC_POINTS_H_

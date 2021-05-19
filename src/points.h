//
// Created by natejh on 5/14/21.
//

#ifndef VORONOI_GENERATION_SRC_POINTS_H_
#define VORONOI_GENERATION_SRC_POINTS_H_

#include "int_matrix.h"

IntMatrix* get_points_in_bounding_box(IntMatrix* box);
IntArray* get_bounding_box_dims(IntMatrix* box);

#endif //VORONOI_GENERATION_SRC_POINTS_H_

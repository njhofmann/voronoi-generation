//
// Created by natejh on 5/14/21.
//

#include <stdio.h>
#include <stdlib.h>
#include "points.h"

IntMatrix* add_bounds_to_int_array(IntArray* arr, int start, int end) {
  int range_size = end - start;
  IntMatrix* expanded = init_int_matrix_from_int_arr(arr, range_size); // inclusive: [start, end]
  for (int i = 0; i < range_size; i++)
    add_to_int_arr(expanded->matrix[i], start + i);
  // TODO double check matrix bounds here?
  expanded->width = expanded->matrix[0]->size;
  return expanded;
}

IntMatrix* get_points_in_bounding_box(IntMatrix* box) {
  /**
   * Returns an IntMatrix of all the points in the given bounding box
   *
   * Ex: if bounding box if from (0,0) to (10, 10) - returns (0, 0), (0, 1), ..., (10, 9), (10, 10)
   */
  if (box->height != 2) {
    fprintf(stderr, "bounding box must have exact two points\n");
    exit(EXIT_FAILURE);
  }

  IntArray* lower_left = box->matrix[0];
  IntArray* upper_right = box->matrix[1];
  int first_dim_size = upper_right->items[0] - lower_left->items[0];
  IntMatrix* points = init_int_matrix(box->width, first_dim_size);

  // assign each item in first dimension (inclusive) to first index of each array in order of appearance
  for (int i = 0; i < first_dim_size; i++)
    add_to_int_arr(points->matrix[i], lower_left->items[0] + i);

  // start in second dimension
  // for each dimension, add entire range of that dimension to each item in the existing array of points
  for (int dim = 1; dim < box->width; dim++) {
    int matrix_count = points->height;
    IntMatrix* temps[matrix_count];// = malloc(sizeof(IntMatrix*) * matrix_count);
    for (int i = 0; i < points->height; i++)
      temps[i] = add_bounds_to_int_array(points->matrix[i], lower_left->items[dim], upper_right->items[dim]);
    free_int_matrix(points);
    points = concat_int_matrices(temps, matrix_count);
  }

  return points;
}

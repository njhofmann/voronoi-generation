//
// Created by natejh on 5/14/21.
//

#include <stdio.h>
#include <stdlib.h>
#include "points.h"

IntMatrix* add_bounds_to_int_array(IntArray* arr, int end) {
  IntMatrix* expanded = init_int_matrix_from_int_arr(arr, end); // inclusive: [start, end]
  for (int i = 0; i < end; i++)
    add_to_int_arr(expanded->matrix[i], i);
  // TODO double check matrix bounds here?
  expanded->width = expanded->matrix[0]->size;
  return expanded;
}

void valid_centers(IntArray* box, IntMatrix* centers) {
  for (int i = 0; i < centers->height; i++) {
    IntArray* cur_center = centers->matrix[i];
    for (int j = 0; j < cur_center->size; j++) {
      int cur_dim = box->items[j];
      int cur_item = cur_center->items[j];
      if (cur_dim <= cur_item) {
        fprintf(stderr, "%d-th item of %d-th center %d is not between [0, %d)", j, i, cur_item, cur_dim);
        exit(EXIT_FAILURE);
      }
    }
  }
}

IntMatrix* get_points_in_bounding_box(IntArray* box) {
  IntArray* upper_right = box;
  int first_dim_size = upper_right->items[0];
  IntMatrix* points = init_int_matrix(box->size, first_dim_size);

  // assign each item in first dimension (inclusive) to first index of each array in order of appearance
  for (int i = 0; i < first_dim_size; i++)
    add_to_int_arr(points->matrix[i], i);

  // start in second dimension
  // for each dimension, add entire range of that dimension to each item in the existing array of points
  for (int dim = 1; dim < box->size; dim++) {
    int matrix_count = points->height;
    IntMatrix* temps[matrix_count];// = malloc(sizeof(IntMatrix*) * matrix_count);
    for (int i = 0; i < points->height; i++)
      temps[i] = add_bounds_to_int_array(points->matrix[i], upper_right->items[dim]);
    free_int_matrix(points);
    points = concat_int_matrices(temps, matrix_count);
  }

  return points;
}

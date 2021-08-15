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

IntArray* get_bounding_box_dims(IntMatrix* box) {
  IntArray* dims = init_int_array(box->width); // TODO is this set
  for (int i = 0; i < box->width; i++)
    add_to_int_arr(dims, box->matrix[1]->items[i] - box->matrix[0]->items[i]);
  // TODO is this correct?
  return dims;
}

void valid_centers(IntMatrix* box, IntMatrix* centers) {
  for (int i = 0; i < centers->height; i++) {
    IntArray* cur_center = centers->matrix[i];
    for (int j = 0; j < cur_center->size; j++) {
      int cur_dim_low = box->matrix[0]->items[j];
      int cur_dim_high = box->matrix[1]->items[j];
      int cur_item = cur_center->items[j];
      if (cur_item < cur_dim_low || cur_dim_high <= cur_item) {
        fprintf(stderr, "%d-th item of %d-th center %d is not between [%d, %d)",
                j, i, cur_item, cur_dim_low, cur_dim_high);
        exit(EXIT_FAILURE);
      }
    }
  }
}

IntMatrix* get_points_in_bounding_box(IntMatrix* box) {
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

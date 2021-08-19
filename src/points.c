//
// Created by natejh on 5/14/21.
//

#include <stdio.h>
#include <stdlib.h>
#include "points.h"

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

IntArray* increment_counter(IntArray* counter, IntArray* box) {
  int last_idx = counter->size-1;
  bool inc_next_dim = false;
  for (int i = last_idx; i >= 0; i--) {
    int cur_dim_cnt = counter->items[i];
    if (i == last_idx || inc_next_dim)
      cur_dim_cnt += 1;

    if (cur_dim_cnt == box->items[i]) {
      cur_dim_cnt = 0;
      inc_next_dim = true;
    }

    counter->items[i] = cur_dim_cnt;

    if (!inc_next_dim)
      break;
  }
  return counter;
}

IntMatrix* get_points_in_bounding_box(IntArray* box) {
  int num_of_points = 1;
  for (int i = 0; i < box->size; i++)
    num_of_points = num_of_points * box->items[i];

  IntArray* counter = init_int_array(box->size);
  counter->size = box->size;
  IntMatrix* points = init_int_matrix_from_int_arr(counter, num_of_points);

  for (int i = 1; i < num_of_points; i++) {
    counter = increment_counter(counter, box);
    copy_int_arr(counter, points->matrix[i]);
  }

  free_int_array(counter);
  return points;
}

//
// Created by natejh on 1/5/21.
//

#ifndef VORONOI_GENERATION__INT_MATRIX_H_
#define VORONOI_GENERATION__INT_MATRIX_H_

#include "int_array.h"
typedef struct {
  IntArray** matrix;
  int total_height; // num of int arrays and unused spaced for int arrays
  int width; // size of each int array
  int height; // number of int arrays
} IntMatrix;

IntMatrix* init_int_matrix(int width, int height);
IntMatrix* init_int_matrix_from_int_arr(IntArray* arr, int height);
IntMatrix* concat_int_matrix(IntArray** arrs);
void add_int_matrix(IntMatrix* matrix, IntArray* arr);
void free_int_matrix(IntMatrix* matrix);
void insert_int_matrix(IntMatrix* matrix, int col, int row, int item);

#endif //VORONOI_GENERATION__INT_MATRIX_H_

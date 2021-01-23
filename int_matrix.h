//
// Created by natejh on 1/5/21.
//

#ifndef VORONOI_GENERATION__INT_MATRIX_H_
#define VORONOI_GENERATION__INT_MATRIX_H_

#include "int_array.h"
typedef struct {
  IntArray** matrix;
  int total_height; // num of spaces ofint arrays and unused spaced for int arrays
  int width; // size of each int array
  int height; // number of inserted int arrays
} IntMatrix;

IntMatrix* init_int_matrix(int width, int height);
IntMatrix* init_empty_int_matrix(int height);
IntMatrix* init_int_matrix_from_int_arr(IntArray* arr, int height);
IntMatrix* init_int_matrix_from_int_arrs(IntArray** arrs, int count);
void add_int_matrix(IntMatrix* matrix, IntArray* arr);
void free_int_matrix(IntMatrix* matrix);
void free_int_matrix_no_data(IntMatrix* matrix);
IntMatrix* concat_int_matrices(IntMatrix** matrices, int size);
void print_int_matrix(IntMatrix* matrix);

#endif //VORONOI_GENERATION__INT_MATRIX_H_

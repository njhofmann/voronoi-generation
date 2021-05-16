//
// Created by natejh on 1/5/21.
//

#ifndef VORONOI_GENERATION__INT_MATRIX_H_
#define VORONOI_GENERATION__INT_MATRIX_H_

#include "int_array.h"

/**
 * Array of IntArrays, resizeable height wise but fixed width wise (all IntArrays must have same width)
 */
typedef struct {
  IntArray** matrix; // IntArrays of matrix
  int total_height;  // allocated + unallocated slots for IntArrays
  int width;         // size of each IntArray
  int height;        // number of inserted IntArrays
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
void write_int_matrix(IntMatrix* matrix, FILE* output_file);

#endif //VORONOI_GENERATION__INT_MATRIX_H_

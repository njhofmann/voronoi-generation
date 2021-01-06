//
// Created by natejh on 1/5/21.
//

#ifndef VORONOI_GENERATION__INT_MATRIX_H_
#define VORONOI_GENERATION__INT_MATRIX_H_

#include "int_array.h"
typedef struct {
  IntArray** matrix;
  int width; // size of each int array
  int height; // number of int arrays
} IntMatrix;

IntMatrix* init_int_matrix(int width, int height);
void free_int_matrix(IntMatrix* matrix);
void insert_int_matrix(IntMatrix* matrix, int i, int j, int item);

#endif //VORONOI_GENERATION__INT_MATRIX_H_

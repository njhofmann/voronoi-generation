//
// Created by natejh on 5/18/21.
//

#ifndef VORONOI_GENERATION_SRC_INT_TENSOR_H_
#define VORONOI_GENERATION_SRC_INT_TENSOR_H_

#include <stdio.h>
#include "int_matrix.h"

typedef struct {
  IntMatrix** tensor;
  int width;   // size of IntArray
  int height;  // size of IntMatrix
  int length;  // # of IntMatrices
  int total_length; // total # of IntMatrices
} IntTensor;

IntTensor* init_int_tensor(IntMatrix* matrix, int length);
IntTensor* add_matrix_to_int_tensor(IntTensor* tensor, IntMatrix* matrix);
void free_int_tensor(IntTensor* tensor);
void write_int_tensor(IntTensor* tensor, FILE* output_file);


#endif //VORONOI_GENERATION_SRC_INT_TENSOR_H_

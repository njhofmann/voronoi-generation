//
// Created by natejh on 5/18/21.
//

#include <malloc.h>
#include <math.h>
#include <string.h>
#include "int_tensor.h"

IntTensor* init_int_tensor(IntMatrix* matrix, int length) {
  IntTensor* tensor = malloc(sizeof(IntTensor));
  tensor->tensor = malloc(sizeof(IntMatrix*) * length);
  tensor->length = 0;
  tensor->total_length = length;
  tensor->width = matrix->width;
  tensor->height = matrix->height;
  return tensor;
}

IntTensor* expand_tensor_length(IntTensor* tensor) {
  int new_total_length = ceil(tensor->total_length * 1.4);
  IntMatrix** new_tensor = malloc(sizeof(IntMatrix*) * new_total_length);
  memcpy(new_tensor, tensor->tensor, sizeof(IntMatrix*) * tensor->length);
  free(tensor->tensor);
  tensor->tensor = new_tensor;
  tensor->total_length = new_total_length;
  return tensor;
}

IntTensor* add_matrix_to_int_tensor(IntTensor* tensor, IntMatrix* matrix) {
  if (tensor->length == tensor->total_length) {
    tensor = expand_tensor_length(tensor);
  }

  tensor->tensor[tensor->length] = matrix;
  tensor->length++;
}

void free_int_tensor(IntTensor* tensor) {
  for (int i = 0; i < tensor->length; i++)
    free_int_matrix(tensor->tensor[i]);
  free(tensor->tensor);
  free(tensor);
}

void write_int_tensor(IntTensor* tensor, FILE* output_file) {
  for (int i = 0; i < tensor->length; i++) {
    write_int_matrix(tensor->tensor[i], output_file);
    fprintf(output_file, "\n");
  }
}


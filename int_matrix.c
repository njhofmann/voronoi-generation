//
// Created by natejh on 1/5/21.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "int_matrix.h"

IntMatrix* init_int_matrix(int width, int height) {
  if (width < 0 || height < 0) {
    fprintf(stderr, "width and height of matrix must be > 0");
    exit(EXIT_FAILURE);
  }

  IntMatrix* matrix = malloc(sizeof(IntMatrix));
  matrix->width = 0; // TODO arrays are initally empty
  matrix->height = height;
  matrix->matrix = malloc(sizeof(IntArray) * height);

  for (int i = 0; i < height; i++)
    matrix->matrix[i] = init_int_array(width);

  return matrix;
}

void free_int_matrix(IntMatrix* matrix) {
  for (int i = 0; i < matrix->height; i++)
    free_int_array(matrix->matrix[i]);
  free(matrix->matrix);
  free(matrix);
}

void add_int_matrix(IntMatrix* matrix, IntArray* arr) {
  if (matrix->width != arr->size) {
    fprintf(stderr, "inserted array of size %d must be same size %d as width of the matrix",
            arr->size, matrix->width);
    exit(EXIT_FAILURE);
  }

  // TODO resizing here
}

IntMatrix* init_int_matrix_from_int_arr(IntArray* arr, int height) {
  IntMatrix* matrix = init_int_matrix(arr->size, height); // TODO total size vs size?
  for (int i = 0; i < matrix->width; i++)
    copy_int_arr(arr, matrix->matrix[i]);
  return matrix;
}

void insert_int_matrix(IntMatrix* matrix, int col, int row, int item) {
  if (col < 0 || col > matrix->width) {
    fprintf(stderr, "column index %d out of bounds", col);
    exit(EXIT_FAILURE);
  }
  else if (row < 0 || row > matrix->height) {
    fprintf(stderr, "row index %d out of bounds", row);
    exit(EXIT_FAILURE);
  }
  matrix->matrix[row]->items[col] = item; // TODO insert item array
}

IntMatrix* concat_int_matrices(IntMatrix** matrices, int size) {
  /**
   * Concatenates all IntMatrixs in the given array to the first IntMatrix, freeing the array and all copied IntMatrixs
   * (but keeping their inner IntArrays intact). Assumes all IntMatrixs have the same width.
   */
  if (size < 1) {
    fprintf(stderr, "array of IntMatrixs must have one IntMatrix");
    exit(EXIT_FAILURE);
  }

  // get total size of new matrix
  IntMatrix* final = matrices[0];
  int new_total_size = 0;
  for (int i = 0; i < size; i++)
    new_total_size += matrices[i]->height;

  // create new empty matrix to fit all matrices, copy over 1st matrix, free original matrix (minus data)
  IntArray** new_inner_matrix = malloc(sizeof(IntArray*) * new_total_size);
  memcpy(new_inner_matrix, final->matrix, sizeof(IntArray*) * final->height);
  int copy_start_idx = final->height;
  free(final->matrix);
  final->matrix = new_inner_matrix;
  final->height = new_total_size;

  // copy over each matrix after the first
  for (int i = 1; i < size; i++) {
    IntMatrix* cur_matrix = matrices[i];
    memcpy(final->matrix[copy_start_idx],
           cur_matrix->matrix + copy_start_idx,  // pointer arithmetic to grab address of where to start copying
        // the next matrix
           sizeof(IntArray*) * cur_matrix->height);
    free(matrices[i]);
  }

  free(matrices);
  return final;
}
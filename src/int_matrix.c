//
// Created by natejh on 1/5/21.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "int_matrix.h"

static const int UNSET_MATRIX_WIDTH = -1;
static const double RESIZE_FACTOR = 1.4;

IntMatrix* init_int_matrix(int width, int height) {
  /**
   * Initializes an IntMatrix to with `height` number of empty IntArrays, each with an `width` number of empty slots
   */
  if (width < 0 || height < 0) {
    fprintf(stderr, "width and height of matrix must be > 0");
    exit(EXIT_FAILURE);
  }

  IntMatrix* matrix = malloc(sizeof(IntMatrix));
  matrix->width = 0; // TODO arrays are initally empty
  matrix->height = height;
  matrix->total_height = height;
  matrix->matrix = malloc(sizeof(IntArray*) * height);

  for (int i = 0; i < height; i++)
    matrix->matrix[i] = init_int_array(width);

  return matrix;
}

void free_int_matrix_no_data(IntMatrix* matrix) {
  /**
   * Frees the memory of the given IntMatrix that has no IntArrays
   */
  free(matrix->matrix);
  free(matrix);
}

void free_int_matrix(IntMatrix* matrix) {
  /**
   * Frees the memory of the given IntMatrix and its contents
   */
  for (int i = 0; i < matrix->height; i++)
    free_int_array(matrix->matrix[i]);
  free_int_matrix_no_data(matrix);
}

IntMatrix* init_int_matrix_from_int_arrs(IntArray** arrs, int count) {
  /**
   * Creates an IntMatrix from the `count` number of IntArrays, checks that they are all of the same size
   */
  for (int i = 1; i < count; i++) {
    int prev_sz = arrs[i]->size;
    int next_sz = arrs[i-1]->size;
    if (prev_sz != next_sz) {
      fprintf(stderr, "inserted int arrays must be of the same size, not %d and %d", prev_sz, next_sz);
      exit(EXIT_FAILURE);
    }
  }

  IntMatrix* matrix = malloc(sizeof(IntMatrix));
  matrix->width = arrs[0]->size;
  matrix->height = count;
  matrix->total_height = count;
  matrix->matrix = arrs;
  return matrix;
}

IntMatrix* expand_int_matrix(IntMatrix* matrix) {
  /**
   * Add more unallocated to the given IntMatrix
   */
  int new_total_height = ceil(matrix->total_height * RESIZE_FACTOR);
  IntArray** new_matrix = malloc(sizeof(IntArray*) * new_total_height);
  memcpy(new_matrix, matrix->matrix, sizeof(IntArray*) * matrix->height);
  free(matrix->matrix);
  matrix->matrix = new_matrix;
  matrix->total_height = new_total_height;
  return matrix;
}

void add_int_matrix(IntMatrix* matrix, IntArray* arr) {
  /**
   * Assigns the given IntArray to the first available slot in the given IntMatrix, if insufficient room - first
   * expands IntMatrix
   */
  if (matrix->width != UNSET_MATRIX_WIDTH && matrix->width != arr->size) {
    fprintf(stderr, "inserted array's size %d must equal matrix width %d", arr->size, matrix->width);
    exit(EXIT_FAILURE);
  }

  if (matrix->width == UNSET_MATRIX_WIDTH)
    matrix->width = arr->size;

  if (matrix->height == matrix->total_height)
    matrix = expand_int_matrix(matrix);

  // assign pointer, doesn't copy
  matrix->matrix[matrix->height] = arr;
  matrix->height++;
}

IntMatrix* init_int_matrix_from_int_arr(IntArray* arr, int height) {
  /**
   * Initializes a IntMatrix by copying the given IntArray `height` number of times
   */
  IntMatrix* matrix = init_int_matrix(arr->size, height); // TODO total size vs size?
  for (int i = 0; i < height; i++)
    copy_int_arr(arr, matrix->matrix[i]);
  return matrix;
}

void print_int_matrix(IntMatrix* matrix) {
  /**
   * Prints each row of an IntMatrix to stdout
   */
  for (int i = 0; i < matrix->height; i++)
    print_int_arr(matrix->matrix[i]);
}

IntMatrix* init_empty_int_matrix(int height) {
  /**
   * Inits an empty IntMatrix with no contents and unknown width
   */
  IntMatrix* matrix = malloc(sizeof(IntMatrix));
  matrix->total_height = height;
  matrix->width = -1;
  matrix->height = 0;
  matrix->matrix = malloc(sizeof(IntArray*) * height);
  return matrix;
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
  int new_total_size = 0;
  for (int i = 0; i < size; i++)
    new_total_size += matrices[i]->height;

  // create new empty matrix to fit all matrices, copy over 1st matrix, free original matrix (minus data)
  IntMatrix* final = matrices[0];
  IntArray** new_inner_matrix = malloc(sizeof(IntArray*) * new_total_size);
  memcpy(new_inner_matrix, final->matrix, sizeof(IntArray*) * final->height);
  int copy_start_idx = final->height;
  free(final->matrix);
  final->matrix = new_inner_matrix;
  final->height = new_total_size;

  // copy over each matrix after the first
  for (int i = 1; i < size; i++) {
    IntMatrix* cur_matrix = matrices[i];
    // pointer arithmetic to find location that was last left
    memcpy(final->matrix + copy_start_idx, cur_matrix->matrix, sizeof(IntArray*) * cur_matrix->height);
    copy_start_idx += cur_matrix->height;
    free(cur_matrix->matrix);
    free(cur_matrix);
  }

  return final;
}

void write_int_matrix(IntMatrix* matrix, FILE* output_file) {
  /**
   * Writes the contents of the given IntMatrix to the given FILE stream
   */
  for (int i = 0; i < matrix->height; i++) {
    write_int_arr(matrix->matrix[i], output_file);
    fputc(' ', output_file);
  }
}
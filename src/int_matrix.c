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
  if (width < 0 || height < 0) {
    fprintf(stderr, "width and height of matrix must be > 0");
    exit(EXIT_FAILURE);
  }

  IntMatrix* matrix = malloc(sizeof(IntMatrix));
  matrix->width = 0; // arrays are initially empty
  matrix->height = height;
  matrix->total_height = height;
  matrix->matrix = malloc(sizeof(IntArray*) * height);

  for (int i = 0; i < height; i++)
    matrix->matrix[i] = init_int_array(width);

  return matrix;
}

void free_int_matrix_no_data(IntMatrix* matrix) {
  free(matrix->matrix);
  free(matrix);
}

void free_int_matrix(IntMatrix* matrix) {
   if (matrix != NULL) {
     for (int i = 0; i < matrix->height; i++)
       free_int_array(matrix->matrix[i]);
     free_int_matrix_no_data(matrix);
   }
}

IntMatrix* init_int_matrix_from_int_arrs(IntArray** arrs, int count) {
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

/**
 * Add more unallocated space to the given IntMatrix
 */
IntMatrix* expand_int_matrix(IntMatrix* matrix) {
  int new_total_height = ceil(matrix->total_height * RESIZE_FACTOR);
  IntArray** new_matrix = malloc(sizeof(IntArray*) * new_total_height);
  memcpy(new_matrix, matrix->matrix, sizeof(IntArray*) * matrix->height);
  free(matrix->matrix);
  matrix->matrix = new_matrix;
  matrix->total_height = new_total_height;
  return matrix;
}

void add_int_matrix(IntMatrix* matrix, IntArray* arr) {
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
  IntMatrix* matrix = init_int_matrix(arr->size, height); // TODO total size vs size?
  for (int i = 0; i < height; i++)
    copy_int_arr(arr, matrix->matrix[i]);
  matrix->width = arr->size;
  return matrix;
}

void print_int_matrix(IntMatrix* matrix) {
  for (int i = 0; i < matrix->height; i++)
    print_int_arr(matrix->matrix[i]);
}

IntMatrix* init_empty_int_matrix(int height) {
  IntMatrix* matrix = malloc(sizeof(IntMatrix));
  matrix->total_height = height;
  matrix->width = -1;
  matrix->height = 0;
  matrix->matrix = malloc(sizeof(IntArray*) * height);
  return matrix;
}

void write_int_matrix(IntMatrix* matrix, FILE* output_file) {
  for (int i = 0; i < matrix->height; i++) {
    write_int_arr(matrix->matrix[i], output_file);
    fputc(' ', output_file);
  }
}
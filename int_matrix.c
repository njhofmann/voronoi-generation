//
// Created by natejh on 1/5/21.
//

#include <stdlib.h>
#include <stdio.h>
#include "int_matrix.h"

IntMatrix* init_int_matrix(int width, int height) {
  if (width < 0 || height < 0) {
    fprintf(stderr, "width and height of matrix must be > 0");
    exit(EXIT_FAILURE);
  }

  IntMatrix* matrix = malloc(sizeof(IntMatrix));
  matrix->width = width;
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
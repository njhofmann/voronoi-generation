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

void insert_int_matrix(IntMatrix* matrix, int i, int j, int item) {
  if (i < 0 || i > matrix->width) {
    fprintf(stderr, "column index %d out of bounds", i);
    exit(EXIT_FAILURE);
  }
  else if (j < 0 || j > matrix->height) {
    fprintf(stderr, "row index %d out of bounds", j);
    exit(EXIT_FAILURE);
  }

  insert_int_array(matrix->matrix[j], i, item);
}
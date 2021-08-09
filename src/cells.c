//
// Created by natejh on 7/26/21.
//

#include "int_tensor.h"
#include <math.h>
#include <stdlib.h>
#include "cells.h"

static const int STARTING_CELL_SIZE = 10;

Cell* init_cell(IntArray* center) {
  /**
   * Create a new Cell from the given IntArray
   */
  Cell* cell = malloc(sizeof(Cell));
  cell->center = center;
  cell->points = init_empty_int_matrix(STARTING_CELL_SIZE);
  return cell;
}

Cells* init_cells(IntMatrix* centers) {
  /**
   * Inits an array of Cells from the given list of IntMatrices
   */
  Cells* cells = malloc(sizeof(Cells) * 1);
  cells->cells = malloc(centers->height * sizeof(Cell*));
  cells->size = centers->height;
  for (int i = 0; i < centers->height; i++)
    cells->cells[i] = init_cell(centers->matrix[i]);
  return cells;
}

IntArray* compute_center(Cell* cell) {
  /**
   * Computes a new center point by averaging each across point in the given Cell, dimension-wise (i.e. average across
   * 1-st index, 2-nd index, ..., i-th index, etc.)
   */
  IntArray* center = init_int_array(cell->points->width);
  center->size = cell->points->width;
  for (int i = 0; i < cell->points->height; i++)
    for (int j = 0; j < center->size; j++)
      center->items[j] += cell->points->matrix[i]->items[j];

  for (int j = 0; j < center->size; j++)
    center->items[j] = (int) round(center->items[j] / (1.0 * cell->points->height));

  return center;
}
IntMatrix* compute_centers(Cells* cells, int process_cnt) {
  /**
   * Computes a center point for each given Cell
   */
  IntMatrix* centers = init_empty_int_matrix(cells->size);
  if (process_cnt == 1) {
    for (int i = 0; i < cells->size; i++)
      add_int_matrix(centers, compute_center(cells->cells[i]));
  }
  else {
    // TODO multiprocessing here
  }
  return centers;
}

void print_cell(Cell* cell, FILE* output_file) {
  /**
   * Writes the given Cell to the given FILE stream in the following manner
   *
   * center | a1,a2,a3 b1,b2,b3 ...
   */
  write_int_arr(cell->center, output_file);
  fprintf(output_file, " | ");
  write_int_matrix(cell->points, output_file);
  fputc('\n', output_file);
}

void print_cells(Cells* cells, FILE* output_file) {
  /**
   * Prints the given Cells to the given FILE stream
   */
  for (int i = 0; i < cells->size; i++)
    print_cell(cells->cells[i], output_file);
  fputc('\n', output_file);
}

void free_cell(Cell* cell) {
  /**
   * Free the given Cell and its underlying IntMatrix, but not the center point or the points underlying the IntMatrix
   */
  free_int_matrix_no_data(cell->points);
  free(cell);
}

void free_cells(Cells* cells) {
  /**
   * Frees the given array of Cells, but not the underlying IntArrays. Assumes the latter are freed by other data
   * structures.
   */
  for (int i = 0; i < cells->size; i++)
    free_cell(cells->cells[i]);
  free(cells->cells);
  free(cells);
}
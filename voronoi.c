//
// Created by natejh on 1/6/21.
//

#include <stdlib.h>
#include "voronoi.h"

static const int STARTING_CELL_SIZE = 10;

int closest_center(IntArray* point, IntMatrix* centers, DistanceMetric distance_metric) {
  /**
   * Finds the index of the center that is closest to the given point via the given distance metric. Assumes the point
   * and centers are of the same dimensionality
   */
  int closest_idx = -1;
  double closest_dist = 0.0;
  for (int i = 0; i < centers->height; i++) {
    double cur_dist = compute_distance_metric(point, centers->matrix[i], distance_metric);
    if (closest_idx < 0 || closest_dist > cur_dist) {
      closest_idx = i;
      closest_dist = cur_dist;
    }
  }
  return closest_idx;
}

Cell* init_cell(IntArray* center) {
  Cell* cell = malloc(sizeof(Cell));
  cell->center = center;
  cell->points = init_empty_int_matrix(STARTING_CELL_SIZE);
  return cell;
}

Cells* init_cells(IntMatrix* centers) {
  Cells* cells = malloc(sizeof(Cells));
  cells->cells = malloc(centers->height * sizeof(Cell*));
  cells->size = centers->height;
  for (int i = 0; i < centers->height; i++)
    cells->cells[i] = init_cell(centers->matrix[i]);
  return cells;
}

Cells* create_voronoi_diagram(IntMatrix* centers, IntMatrix* points, DistanceMetric metric) {
  Cells* cells = init_cells(centers);
  // TODO multithreading?
  for (int i = 0; i < points->height; i++) {
    IntArray* cur_point = points->matrix[i];
    int closest_center_idx = closest_center(cur_point, centers, metric);
    add_int_matrix(cells->cells[closest_center_idx]->points, cur_point);
  }
  return cells;
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
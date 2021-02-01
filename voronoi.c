//
// Created by natejh on 1/6/21.
//

#include <stdlib.h>
#include <math.h>
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

IntArray* compute_center(Cell* cell) {
  IntArray* center = init_int_array(cell->points->width);
  center->size = cell->points->width;
  for (int i = 0; i < cell->points->height; i++)
    for (int j = 0; j < center->size; j++)
      center->items[j] += cell->points->matrix[i]->items[j];

  for (int j = 0; j < center->size; j++)
    center->items[j] = round(center->items[j] / (1.0 * cell->points->height));

  return center;
}

IntMatrix* compute_centers(Cells* cells) {
  IntMatrix* centers = init_empty_int_matrix(cells->size);
  for (int i = 0; i < cells->size; i++)
    add_int_matrix(centers, compute_center(cells->cells[i]));
  return centers;
}

void print_cell(Cell* cell, FILE* output_file) {
  // TODO how to print cell
}

void print_cells(Cells* cells, FILE* output_file) {
  for (int i = 0; i < cells->size; i++)
    print_cell(cells->cells[i], output_file);

  for (int i = 0; i < 2; i++)
    fputc('\n', output_file);
}

void voronoi_relaxation(IntMatrix* points, IntMatrix* centers, DistanceMetric metric, int iterations,
                        double convergence, FILE* stream) {
  double delta_dist = 0.0;
  while ((convergence > 0.0 && delta_dist > convergence) || iterations > 0) {
    Cells* voronoi_diagram = create_voronoi_diagram(centers, points, metric);
    IntMatrix* new_centers = compute_centers(voronoi_diagram);
    print_cells(voronoi_diagram, stream);

    free_cells(voronoi_diagram);
    free_int_matrix(centers);
    centers = new_centers;

    if (iterations >= 0)
      iterations--;
  }
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
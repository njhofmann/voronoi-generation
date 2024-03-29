//
// Created by natejh on 1/6/21.
//

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "cells.h"
#include "voronoi.h"
#include "int_tensor.h"
#include "wrapper_io.h"
#include "closest_center.h"

static const int STARTING_SIZE = 10;

Cells* create_voronoi_diagram(IntMatrix* centers, IntMatrix* points, DistanceMetric metric, int p, int k) {
  Cells* cells = init_cells(centers);
  for (int i = 0; i < points->height; i++) {
    IntArray* cur_point = points->matrix[i];
    int closest_center_idx = k_th_closest_center(cur_point, centers, metric, p, k);
    add_int_matrix(cells->cells[closest_center_idx]->points, cur_point);
  }

  // edge case: when duplicate centers and only the first instance has points added, add center to itself so there is
  // at least one point to process
  for (int i = 0; i < cells->size; i++) {
    Cell* cur_cell = cells->cells[i];
    if (cur_cell->points->height < 1)
      add_int_matrix(cur_cell->points, cur_cell->center);
  }

  return cells;
}

double center_dist(IntArray* a, IntArray* b) {
  return compute_distance_metric(a, b, EUCLIDEAN, 2);
}

/**
 * Returns if the given convergence threshold has been met - i.e. is there a pair of index matching centers (i-th
 * center in each group) that has moved < `converge_threshold`
 */
bool convergence_threshold_met(double converge_threshold, IntMatrix* old_centers, IntMatrix* new_centers) {
  if (converge_threshold < 0.0)
    return false;
  for (int i = 0; i < old_centers->height; i++)
    if (converge_threshold > center_dist(old_centers->matrix[i], new_centers->matrix[i]))
      return true;
  return false;
}

/**
 * When mapping the given point and all other points in the given dimensions to a 1D array, returns the position of that
 * array
 */
int get_point_idx(IntArray* point, IntArray* dims) {
  // no clue why this works :)
  int t = 0;
  for (int i = 0; i < point->size - 1; i++)
    t += point->items[i] * dims->items[i + 1];
  return t + point->items[point->size - 1];
}

IntMatrix* record_point_assigns(IntArray* dims, IntMatrix* point_groups, Cells* cells) {
  for (int i = 0; i < cells->size; i++) {
    Cell* cur_cell = cells->cells[i];
    for (int j = 0; j < cur_cell->points->height; j++) {
      IntArray* cur_point = cur_cell->points->matrix[j];
      // points may not be in order, need to get proper index
      int cur_point_idx = get_point_idx(cur_point, dims);
      add_to_int_arr(point_groups->matrix[cur_point_idx], i);
    }
  }
  point_groups->width++;
  return point_groups;
}

char* add_str(char* src, const char* add) {
  char* dest = calloc(strlen(src) + strlen(add) + 2, sizeof(char));
  dest = strcpy(dest, src);
  dest = strcat(dest, "/");
  return strcat(dest, add);
}

void write_point_centers(IntMatrix* points, IntMatrix* point_centers, IntArray* dims, char* output_dirc) {
  FILE* stream = stdout;
  if (output_dirc != NULL) {
    char* file = add_str(output_dirc, "point_centers.txt");
    stream = fopen(file, "w");
    free(file);
  }

  for (int i = 0; i < points->height; i++) {
    IntArray* cur_point = points->matrix[i];
    int cur_point_idx = get_point_idx(cur_point, dims);
    write_int_arr(cur_point, stream);
    fputc(' ', stream);
    IntArray* cur_centers = point_centers->matrix[cur_point_idx];
    for (int j = 0; j < point_centers->width; j++) {
      fprintf(stream, "%d", cur_centers->items[j]);
      fputc(' ', stream);
    }
    fputc('\n', stream);
  }

  (stream != stdout) ? fclose(stream) : putc('\n', stream);
}

void write_all_centers(IntTensor* all_centers, char* output_dirc) {
  FILE* stream = stdout;
  if (output_dirc != NULL) {
    char* file = add_str(output_dirc, "centers.txt");
    stream = fopen(file, "w");
    free(file);
  }
  write_int_tensor(all_centers, stream);
  (stream != stdout) ? fclose(stream) : putc('\n', stream);
}

void voronoi_relaxation(IntArray* dimensions, IntMatrix* points, IntMatrix* centers, DistanceMetric metric,
                        int iterations, double converge_threshold, char* output_dirc, bool full_output,
                        int process_cnt, int p, int k) {
  // holds each point's assigned cell during each iteration
  IntMatrix* point_centers = init_int_matrix(STARTING_SIZE, points->height);

  // holds computed centers from each iteration
  IntTensor* all_centers = init_int_tensor(centers, STARTING_SIZE);

  bool converged = false;
  IntMatrix* new_centers;
  bool finished = false;
  while (!finished) {
    Cells* voronoi_diagram = create_voronoi_diagram(centers, points, metric, p, k);
    new_centers = compute_centers(voronoi_diagram, process_cnt);

    if (converge_threshold > 0)
      converged = convergence_threshold_met(converge_threshold, centers, new_centers);

    if (iterations > 0)
      iterations--;

    finished = iterations == 0 || converged;

    if (full_output || finished) {
      point_centers = record_point_assigns(dimensions, point_centers, voronoi_diagram);
      add_matrix_to_int_tensor(all_centers, centers);
    }

    free_cells(voronoi_diagram);
    centers = new_centers;
  }

  write_point_centers(points, point_centers, dimensions, output_dirc);
  write_all_centers(all_centers, output_dirc);

  if (new_centers != NULL)
    free_int_matrix(new_centers);

  free_int_tensor(all_centers);
  free_int_matrix(point_centers);
}
//
// Created by natejh on 1/6/21.
//

#include <stdlib.h>
#include "voronoi.h"

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

Cell** voronoi_diagram(IntMatrix* centers, IntMatrix* points, DistanceMetric metric) {
  Cell** cells = malloc(centers->height * sizeof(Cell*));
  for (int i = 0; i < centers->height; i++) {
    cells[i] = malloc(sizeof(Cell*));
    cells[i]->center = centers->matrix[i];
  }

  // TODO multithreading?
  for (int i = 0; i < points->height; i++) {
    IntArray* cur_point = points->matrix[i];
    int closest_center_idx = closest_center(cur_point, centers, metric);
    add_int_matrix(cells[closest_center_idx]->points, cur_point);
  }

  return cells;
}
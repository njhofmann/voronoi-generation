//
// Created by natejh on 1/6/21.
//

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include "voronoi.h"
#include "int_tensor.h"

static const int STARTING_CELL_SIZE = 10;

int closest_center(IntArray* point, IntMatrix* centers, DistanceMetric distance_metric, int p) {
  /**
   * Finds the index of the center that is closest to the given point via the given distance metric. Assumes the point
   * and centers are of the same dimensionality
   */
  int closest_idx = -1;
  double closest_dist = 0.0;
  for (int i = 0; i < centers->height; i++) {
    if (same_int_arr(point, centers->matrix[i]))
      return i;

    double cur_dist = compute_distance_metric(point, centers->matrix[i], distance_metric, p);
    if (closest_idx < 0 || closest_dist > cur_dist) {
      closest_idx = i;
      closest_dist = cur_dist;
    }
  }
  return closest_idx;
}

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
  Cells* cells = malloc(sizeof(Cells));
  cells->cells = malloc(centers->height * sizeof(Cell*));
  cells->size = centers->height;
  for (int i = 0; i < centers->height; i++)
    cells->cells[i] = init_cell(centers->matrix[i]);
  return cells;
}

Cells* create_voronoi_diagram(IntMatrix* centers, IntMatrix* points, DistanceMetric metric, int p) {
  Cells* cells = init_cells(centers);
  // TODO multithreading / multiprocessing
  // splits `points` across n threads / processes
  for (int i = 0; i < points->height; i++) {
    IntArray* cur_point = points->matrix[i];
    int closest_center_idx = closest_center(cur_point, centers, metric, p);
    add_int_matrix(cells->cells[closest_center_idx]->points, cur_point);
  }

  // edge case when there are duplicate centers and only the first instance has points added
  // add center itself so there is something
  for (int i = 0; i < cells->size; i++) {
    Cell* cur_cell = cells->cells[i];
    if (cur_cell->points->height < 1)
      add_int_matrix(cur_cell->points, cur_cell->center);
  }

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
    center->items[j] = (int)round(center->items[j] / (1.0 * cell->points->height));

  return center;
}

IntMatrix* compute_centers(Cells* cells) {
  /**
   * Computes a center point for each given Cell
   */
  // TODO multithreading vs multiprocessing here
  IntMatrix* centers = init_empty_int_matrix(cells->size);
  for (int i = 0; i < cells->size; i++)
    add_int_matrix(centers, compute_center(cells->cells[i]));
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

double center_dist(IntArray* a, IntArray* b) {
  return compute_distance_metric(a, b, EUCLIDEAN, 2);
}

bool convergence_threshold_met(double converge_threshold, IntMatrix* old_centers, IntMatrix* new_centers) {
  /**
   * Returns if the given convergence threshold has been met - i.e. is there a pair of index matching centers (i-th
   * center in each group) that has moved < `converge_threshold`
   */
  if (converge_threshold < 0.0)
    return false;
  for (int i = 0; i < old_centers->height; i++)
    if (converge_threshold > center_dist(old_centers->matrix[i], new_centers->matrix[i]))
      return true;
  return false;
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

int get_point_idx(IntArray* point, IntArray* dims) {
  // TODO generalize this to n-d
  return (dims->items[1] * point->items[0]) + point->items[1];
}

IntMatrix* record_point_assigns(IntArray* dims, IntMatrix* point_groups, Cells* cells) {
  for (int i = 0; i < cells->size; i++) {
    Cell* cur_cell = cells->cells[i];
    for (int j = 0; j < cur_cell->points->height; j++) {
      IntArray* cur_point = cur_cell->points->matrix[j];
      int cur_point_idx = get_point_idx(cur_point, dims);
      add_to_int_arr(point_groups->matrix[cur_point_idx], i);
    }
  }
  point_groups->width++;
  return point_groups;
}

char* add_str(char* src, const char* add) {
  char* dest = calloc(strlen(src) + strlen(add) + 2, sizeof(char ));
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
  fclose(stream);
}

void write_all_centers(IntTensor* all_centers, char* output_dirc) {
  FILE* stream = stdout;
  if (output_dirc != NULL) {
    char* file = add_str(output_dirc, "centers.txt");
    stream = fopen(file, "w");
    free(file);
  }
  write_int_tensor(all_centers, stream);
  fclose(stream);
}

void voronoi_relaxation(IntArray* dimensions, IntMatrix* points, IntMatrix* centers, DistanceMetric metric,
                        int iterations, double converge_threshold, char* output_dirc, bool full_output, int p) {
  /**
   * Executes iterations of Voronoi relaxation from the given set of starting points and centers, using the given
   * DistanceMetric
   *
   * Results are writen to the given FILE stream, if `full_output` is true each iteration is writen
   *
   * If `iterations` is positive, computes that many iterations of relaxation
   *
   * If `converge_threshold` is positive, executes iterations until the center of some cluster moves <
   * `converge_theshold` between iterations
   *
   * If `iterations` and `converge_threshold` used, stops on the first one to be met
   */
  // holds each points assigned cell during each iteration
  IntMatrix* point_centers = init_int_matrix(10, points->height);

  IntTensor* all_centers = init_int_tensor(centers, 10);

  bool converged = false;
  IntMatrix* new_centers;
  bool finished = false;
  while (!finished) {
    Cells* voronoi_diagram = create_voronoi_diagram(centers, points, metric, p);
    new_centers = compute_centers(voronoi_diagram);

    if (converge_threshold > 0)
      converged = convergence_threshold_met(converge_threshold, centers, new_centers);

    if (iterations > 0)
      iterations--;

    finished = iterations == 0 || converged;

    if (full_output || finished) {
      record_point_assigns(dimensions, point_centers, voronoi_diagram);
      add_matrix_to_int_tensor(all_centers, centers);
    }
      // print_cells(voronoi_diagram, stream);

    free_cells(voronoi_diagram);

    centers = new_centers;
  }

  write_point_centers(points, point_centers, dimensions, output_dirc);
  write_all_centers(all_centers, output_dirc);

  if (new_centers != NULL)
    free_int_matrix(new_centers);

  free_int_tensor(all_centers);
  free_int_matrix(point_centers);

  for (int i = 0; i < points->height; i++)
    free_int_array(points->matrix[i]);
  free(points->matrix);
  free(points);
}
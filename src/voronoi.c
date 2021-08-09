//
// Created by natejh on 1/6/21.
//

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "cells.h"
#include "voronoi.h"
#include "int_tensor.h"
#include "wrapper_io.h"

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

int* split_array(int array_length, int chunks) {
  /**
   * Returns an array of indices for splitting up an array of the given length into `chunk` number of evenly sized
   * subarrays, i-th index gives the starting index of the i-th chunk
   *
   * If the size of each chunk does not evenly factor into the array length, size of each chunk will be rounded up and
   * the last chunk will be smaller than the earlier chunks
   *
   * Ex:) array of size 10 into two chunks --> {0, 5}
   *      10, 3 --> {0, 4, 8}
   */
  int* chunk_idxs = malloc(sizeof(int) * chunks);
  int chunk_size = ceil((1.0 * array_length) / chunks);
  int i = 0;
  int cur_idx = 0;
  do {
    chunk_idxs[i] = cur_idx;
    cur_idx += chunk_size;
    i++;
  } while (i < chunks);
  return chunk_idxs;
}

int* assign_points(IntMatrix* centers, IntMatrix* points, int start_idx, int end_idx, DistanceMetric metric, int p) {
  int length = end_idx - start_idx;
  int* assigned_centers = malloc(sizeof(int) * length);
  for (int i = 0; i < length; i++) {
    IntArray* cur_point = points->matrix[start_idx + i];
    int closest_center_idx = closest_center(cur_point, centers, metric, p);
    assigned_centers[i] = closest_center_idx;
  }
  return assigned_centers;
}

int** init_child_pipes(int cnt) {
  int** pipes = malloc(sizeof(int*) * cnt);
  for (int i = 0; i < cnt; i++) {
    pipes[i] = malloc(sizeof(int) * 2);
    if (pipe(pipes[i]) == -1) {
      fprintf(stderr, "failed to create child pipes");
      exit(EXIT_FAILURE);
    }
  }
  return pipes;
}

void free_child_pipes(int** pipes, int cnt) {
  for (int i = 0; i < cnt; i++)
    free(pipes[i]);
  free(pipes);
}

int* get_chunk_byte_sizes(int* idxs, int cnt, int arr_len) {
  int* sizes = malloc(sizeof(int) * cnt);
  for (int i = 0; i < cnt; i++) {
    int next_idx = (i == cnt - 1) ? arr_len : idxs[i + 1];
    sizes[i] = sizeof(int) * (next_idx - idxs[i]);
  }
  return sizes;
}

Cells* create_voronoi_diagram(IntMatrix* centers, IntMatrix* points, DistanceMetric metric, int p, int process_cnt) {
  int* chunks_idxs = split_array(points->height, process_cnt);
  int** child_pipes = init_child_pipes(process_cnt);
  int* chunk_byte_sizes = get_chunk_byte_sizes(chunks_idxs, process_cnt, points->height);
  pid_t children[process_cnt];
  for (int i = 0; i < process_cnt; i++) {
    if ((children[i] = fork()) == 0) {
      // close pipes for all other child processes
      for (int j = 0; j < process_cnt; j++) {
        close(child_pipes[j][0]);
        if (i != j)
          close(child_pipes[j][1]);
      }

      int end_idx = (i == process_cnt - 1) ? points->height : chunks_idxs[i + 1];
      int* assigned_centers = assign_points(centers, points, chunks_idxs[i], end_idx, metric, p);
      exact_write(child_pipes[i][1], assigned_centers, chunk_byte_sizes[i]);
      close(child_pipes[i][1]);
      exit(0); // exit so no loop
    }
    else if (children[i] < 0) { // child < 0
      fprintf(stderr, "failed to create child processes");
      exit(1);
    }
  }

//  int** all_assigned_centers = malloc(sizeof(int*) * process_cnt);
//  int start_idx = chunks_idxs[0];
//  int end_idx = (0 == process_cnt - 1) ? points->height : chunks_idxs[0 + 1];
//  int* assigned_centers = assign_points(centers, points, start_idx, end_idx, metric, p);
//  all_assigned_centers[0] = assigned_centers;

  int status;
  for (int i = 0; i < process_cnt; i++) {
    waitpid(children[i], &status, 0);
  }

  // merge cells
  int** all_assigned_centers = malloc(sizeof(int*) * process_cnt);
  for (int i = 0; i < process_cnt; i++) {
    close(child_pipes[i][1]); // close write end
    all_assigned_centers[i] = malloc(chunk_byte_sizes[i]);
    exact_read(child_pipes[i][0], all_assigned_centers[i], chunk_byte_sizes[i]);
    close(child_pipes[i][0]); // close read end
  }

  Cells* cells = init_cells(centers);
  int k = 0;
  for (int i = 0; i < process_cnt; i++) {
    int end_idx = (i == process_cnt - 1) ? points->height : chunks_idxs[i + 1];
    int cur_chunk_size = end_idx - chunks_idxs[i];
    for (int j = 0; j < cur_chunk_size; j++) {
      int cur_center_idx = all_assigned_centers[i][j];
      add_int_matrix(cells->cells[cur_center_idx]->points, points->matrix[k]);
      k++;
    }
  }

  free_child_pipes(child_pipes, process_cnt);
  for (int i = 0; i < process_cnt; i++)
    free(all_assigned_centers[i]);
  free(all_assigned_centers);
  free(chunk_byte_sizes);
  free(chunks_idxs);

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
                        int iterations, double converge_threshold, char* output_dirc, bool full_output,
                        int process_cnt, int p) {
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
    Cells* voronoi_diagram = create_voronoi_diagram(centers, points, metric, p, process_cnt);
    new_centers = compute_centers(voronoi_diagram, process_cnt);

    if (converge_threshold > 0)
      converged = convergence_threshold_met(converge_threshold, centers, new_centers);

    if (iterations > 0)
      iterations--;

    finished = iterations == 0 || converged;

    if (full_output || finished) {
      record_point_assigns(dimensions, point_centers, voronoi_diagram);
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

  for (int i = 0; i < points->height; i++)
    free_int_array(points->matrix[i]);
  free(points->matrix);
  free(points);
}
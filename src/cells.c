//
// Created by natejh on 7/26/21.
//

#include <math.h>
#include <stdlib.h>
#include "util.h"
#include "cells.h"
#include "wrapper_io.h"
#include "pipes.h"

static const int STARTING_CELL_SIZE = 10;

Cell* init_cell(IntArray* center) {
  Cell* cell = malloc(sizeof(Cell));
  cell->center = center;
  cell->points = init_empty_int_matrix(STARTING_CELL_SIZE);
  return cell;
}

Cells* init_cells(IntMatrix* centers) {
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

int* compute_chunk_byte_sizes(int* idxs, int cnt, int arr_len, int dims) {
  int* sizes = malloc(sizeof(int) * cnt);
  for (int i = 0; i < cnt; i++) {
    int next_idx = (i == cnt - 1) ? arr_len : idxs[i + 1];
    sizes[i] = sizeof(int) * (next_idx - idxs[i]) * dims;
  }
  return sizes;
}

IntMatrix* compute_centers(Cells* cells, int process_cnt) {
  IntMatrix* centers = init_empty_int_matrix(cells->size);
  if (process_cnt == 1) {
    for (int i = 0; i < cells->size; i++)
      add_int_matrix(centers, compute_center(cells->cells[i]));
    return centers;
  }

  int* chunks_idxs = split_array(cells->size, process_cnt);
  int** child_pipes = init_pipes(process_cnt);
  int dims = cells->cells[0]->center->size;
  int* chunk_byte_sizes = compute_chunk_byte_sizes(chunks_idxs, process_cnt, cells->size, dims);
  pid_t children[process_cnt];
  for (int i = 0; i < process_cnt; i++) {
    if ((children[i] = fork()) == 0) {
      close_all_other_pipes(child_pipes, process_cnt, i);
      int start_idx = chunks_idxs[i];
      int end_idx = (i == process_cnt - 1) ? cells->size : chunks_idxs[i + 1];
      int* child_centers = malloc(chunk_byte_sizes[i]);
      int idx = 0;
      for (int j = start_idx; j < end_idx; j++) {
        IntArray* center = compute_center(cells->cells[j]);
        for (int k = 0; k < center->size; k++) {
          child_centers[idx] = center->items[k];
          idx++;
        }
        free_int_array(center);
      }
      exact_write(child_pipes[i][1], child_centers, chunk_byte_sizes[i]);
      close(child_pipes[i][1]);
      free(child_centers);
      exit(0);
    }
    else if (children[i] < 0) { // child < 0
      fprintf(stderr, "failed to create child processes");
      exit(1);
    }
  }

  int status;
  for (int i = 0; i < process_cnt; i++) {
    waitpid(children[i], &status, 0);
  }

  // merge centers
  int** raw_centers = malloc(sizeof(int*) * process_cnt);
  for (int i = 0; i < process_cnt; i++) {
    close(child_pipes[i][1]); // close write end
    raw_centers[i] = malloc(chunk_byte_sizes[i]);
    exact_read(child_pipes[i][0], raw_centers[i], chunk_byte_sizes[i]);
    close(child_pipes[i][0]); // close read end
  }

  // int** --> IntMatrix*
  for (int i = 0; i < process_cnt; i++) {
    int end_idx = (i == process_cnt - 1) ? cells->size : chunks_idxs[i + 1];
    int cur_chunk_size = end_idx - chunks_idxs[i];
    int m = 0;
    for (int j = 0; j < cur_chunk_size; j++) {
      IntArray* cur_center = init_int_array(dims);
      for (int k = 0; k < dims; k++) {
        add_to_int_arr(cur_center, raw_centers[i][m]);
        m++;
      }
      add_int_matrix(centers, cur_center);
    }
  }

  free_pipes(child_pipes, process_cnt);
  for (int i = 0; i < process_cnt; i++)
    free(raw_centers[i]);
  free(raw_centers);
  free(chunk_byte_sizes);
  free(chunks_idxs);
  return centers;
}

void print_cell(Cell* cell, FILE* output_file) {
  write_int_arr(cell->center, output_file);
  fprintf(output_file, " | ");
  write_int_matrix(cell->points, output_file);
  fputc('\n', output_file);
}

void print_cells(Cells* cells, FILE* output_file) {
  for (int i = 0; i < cells->size; i++)
    print_cell(cells->cells[i], output_file);
  fputc('\n', output_file);
}

void free_cell(Cell* cell) {
  free_int_matrix_no_data(cell->points);
  free(cell);
}

void free_cells(Cells* cells) {
  for (int i = 0; i < cells->size; i++)
    free_cell(cells->cells[i]);
  free(cells->cells);
  free(cells);
}
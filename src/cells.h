//
// Created by natejh on 7/26/21.
//

#ifndef VORONOI_GENERATION_SRC_CELLS_H_
#define VORONOI_GENERATION_SRC_CELLS_H_

#include "int_array.h"
#include "int_matrix.h"

/**
 * A "cluster" within a Voronoi diagram, i.e. a center point and all points that have it as its closest center point
 */
typedef struct {
  IntArray* center;
  IntMatrix* points;
} Cell;

/**
 * A collection of Cells
 */
typedef struct {
  Cell** cells;
  int size;     // number of cells
} Cells;

Cell* init_cell(IntArray* center);
Cells* init_cells(IntMatrix* centers);
IntMatrix* compute_centers(Cells* cells, int process_cnt);
void print_cell(Cell* cell, FILE* output_file);
void print_cells(Cells* cells, FILE* output_file);
void free_cell(Cell* cell);
void free_cells(Cells* cells);

#endif //VORONOI_GENERATION_SRC_CELLS_H_

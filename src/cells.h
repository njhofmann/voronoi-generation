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

/**
 * Create a new Cell from the given IntArray
 */
Cell* init_cell(IntArray* center);

/**
 * Inits an array of Cells from the given IntMatrix
 */
Cells* init_cells(IntMatrix* centers);

/**
 * Computes a new center point for each given Cell across `process_cnt` number of child processes
 */
IntMatrix* compute_centers(Cells* cells, int process_cnt);

/**
 * Writes the given Cell to the given FILE stream in the following manner
 *
 * center | a1,a2,a3 b1,b2,b3 ...
 */
void print_cell(Cell* cell, FILE* output_file);

/**
 * Prints the given Cells to the given FILE stream
 */
void print_cells(Cells* cells, FILE* output_file);

/**
 * Free the given Cell and its underlying IntMatrix, but not the center point or the points underlying the IntMatrix
 */
void free_cell(Cell* cell);

/**
  * Frees the given array of Cells, but not the underlying IntArrays. Assumes the latter are freed by other data
  * structures.
  */
void free_cells(Cells* cells);

#endif //VORONOI_GENERATION_SRC_CELLS_H_

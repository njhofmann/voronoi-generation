//
// Created by natejh on 1/5/21.
//

#ifndef VORONOI_GENERATION__INT_MATRIX_H_
#define VORONOI_GENERATION__INT_MATRIX_H_

#include "int_array.h"

/**
 * Array of IntArrays, resizeable height wise but fixed width wise (all IntArrays must have same width)
 */
typedef struct {
  IntArray** matrix; // IntArrays of matrix
  int total_height;  // allocated + unallocated slots for IntArrays
  int width;         // size of each IntArray
  int height;        // number of inserted IntArrays
} IntMatrix;

/**
 * Initializes an IntMatrix to with `height` number of empty IntArrays, each with an `width` number of empty slots
 */
IntMatrix* init_int_matrix(int width, int height);

/**
 * Inits an empty IntMatrix with no contents and unknown width
 */
IntMatrix* init_empty_int_matrix(int height);

/**
 * Initializes a IntMatrix by copying the given IntArray `height` number of times
 */
IntMatrix* init_int_matrix_from_int_arr(IntArray* arr, int height);

/**
 * Creates an IntMatrix from the `count` number of IntArrays, checks that they are all of the same size
 */
IntMatrix* init_int_matrix_from_int_arrs(IntArray** arrs, int count);

/**
 * Assigns the given IntArray to the first available slot in the given IntMatrix, if insufficient room - first
 * expands IntMatrix
 */
void add_int_matrix(IntMatrix* matrix, IntArray* arr);

/**
 * Frees the memory of the given IntMatrix and its contents
 */
void free_int_matrix(IntMatrix* matrix);

/**
 * Frees the memory of the given IntMatrix that has no IntArrays
 */
void free_int_matrix_no_data(IntMatrix* matrix);

/**
 * Concatenates all IntMatrixs in the given array to the first IntMatrix, freeing the array and all copied IntMatrixs
 * (but keeping their inner IntArrays intact). Assumes all IntMatrixs have the same width.
 */
IntMatrix* concat_int_matrices(IntMatrix** matrices, int size);

/**
 * Prints each row of an IntMatrix to stdout, for debugging
 */
void print_int_matrix(IntMatrix* matrix);

/**
 * Writes the contents of the given IntMatrix to the given FILE stream
*/
void write_int_matrix(IntMatrix* matrix, FILE* output_file);

#endif //VORONOI_GENERATION__INT_MATRIX_H_

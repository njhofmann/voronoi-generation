//
// Created by natejh on 1/5/21.
//

#ifndef VORONOI_GENERATION__INT_ARRAY_H_
#define VORONOI_GENERATION__INT_ARRAY_H_

#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include <stdbool.h>
/**
 * A resizeable array of ints
 */
typedef struct {
  int* items;     // contents of array
  int total_size; // allocated + unallocated space
  int size;       // allocated space
} IntArray;

/**
 * Initializes a new IntArray with the given amount of unallocated slots
 */
IntArray* init_int_array(int size);

/**
 * Frees the memory of the given IntArray
 */
void free_int_array(IntArray* arr);

/**
  * Adds the given integer to the end of the given IntArray, resizing it if out of room
  */
void add_to_int_arr(IntArray* arr, int item);

/**
  * Copies the contents of the src IntArray to the dest IntArray, does not transfer the amount of unallocated space
  */
void copy_int_arr(IntArray* src, IntArray* dest);

/**
  * Prints the contents of the given IntArray to stdout, for debugging
  */
void print_int_arr(IntArray* arr);

/**
 * Writes the contents of the given IntArray to the given FILE stream
 */
void write_int_arr(IntArray* arr, FILE* file);

/**
 * Returns if the two given IntArrays are the same
 */
bool same_int_arr(IntArray* a, IntArray* b);

#endif //VORONOI_GENERATION__INT_ARRAY_H_

//
// Created by natejh on 1/5/21.
//

#ifndef VORONOI_GENERATION__INT_ARRAY_H_
#define VORONOI_GENERATION__INT_ARRAY_H_

#include <stdbool.h>
/**
 * A resizeable array of ints
 */
typedef struct {
  int* items;     // contents of array
  int total_size; // allocated + unallocated space
  int size;       // allocated space
} IntArray;

IntArray* init_int_array(int size);
void free_int_array(IntArray* arr);
void add_to_int_arr(IntArray* arr, int item);
void copy_int_arr(IntArray* src, IntArray* dest);
void print_int_arr(IntArray* arr);
void write_int_arr(IntArray* arr, FILE* file);
bool same_int_arr(IntArray* a, IntArray* b);
#endif //VORONOI_GENERATION__INT_ARRAY_H_

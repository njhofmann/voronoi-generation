//
// Created by natejh on 1/5/21.
//

#ifndef VORONOI_GENERATION__INT_ARRAY_H_
#define VORONOI_GENERATION__INT_ARRAY_H_

typedef struct {
  int* items;
  int total_size; // used + unused space
  int size;  // used space
} IntArray;

IntArray* init_int_array(int size);
void free_int_array(IntArray* arr);
void add_to_int_arr(IntArray* arr, int item);
void copy_int_arr(IntArray* src, IntArray* dest);
void print_int_arr(IntArray* arr);

#endif //VORONOI_GENERATION__INT_ARRAY_H_

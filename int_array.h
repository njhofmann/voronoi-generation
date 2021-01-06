//
// Created by natejh on 1/5/21.
//

#ifndef VORONOI_GENERATION__INT_ARRAY_H_
#define VORONOI_GENERATION__INT_ARRAY_H_

typedef struct a {
  int* items;
  int size;
} IntArray;

IntArray* init_int_array(int size);
void free_int_array(IntArray* arr);
void insert_int_array(IntArray* arr, int idx, int item);

#endif //VORONOI_GENERATION__INT_ARRAY_H_

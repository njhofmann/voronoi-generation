//
// Created by natejh on 1/5/21.
//
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "int_array.h"

static const double INT_ARR_RESIZE_CONST = 1.4;

IntArray* init_int_array(int size) {
  if (size < 1) {
    fprintf(stderr, "int array must be of size 1");
    exit(EXIT_FAILURE);
  }

  IntArray* arr = malloc(sizeof(IntArray));
  arr->total_size = size;
  arr->items = calloc(size, sizeof(int));
  arr->size = 0;
  return arr;
}

void free_int_array(IntArray* arr) {
  free(arr->items);
  free(arr);
}

void add_to_int_arr(IntArray* arr, int item) {
//  if (idx < 0) {
//    fprintf(stderr, "index %d less than zero for array of size %d", idx, arr->size);
//    exit(EXIT_FAILURE);
//  }
  if (arr->size >= arr->total_size) {
    int new_size = (int)round(INT_ARR_RESIZE_CONST * arr->total_size);
    int* temp = calloc(new_size, sizeof(int));
    memcpy(temp, arr->items, sizeof(int) * arr->total_size);
    free(arr->items);
    arr->total_size = new_size;
    arr->items = temp;
  }
  arr->items[arr->size] = item;
  arr->size++;
}

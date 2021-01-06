//
// Created by natejh on 1/5/21.
//
#include <stdlib.h>
#include <stdio.h>
#include "int_array.h"

IntArray* init_int_array(int size) {
  if (size < 1) {
    fprintf(stderr, "int array must be of size 1");
    exit(EXIT_FAILURE);
  }

  IntArray* arr = malloc(sizeof(IntArray));
  arr->size = size;
  arr->items = malloc(sizeof(int) * size);
  return arr;
}

void free_int_array(IntArray* arr) {
  free(arr->items);
  free(arr);
}

void insert_int_array(IntArray* arr, int idx, int item) {
  if (idx < 0 || idx > arr->size) {
    fprintf(stderr, "index %d out of bounds for array of size %d", idx, arr->size);
    exit(EXIT_FAILURE);
  }
  arr->items[idx] = item;
}

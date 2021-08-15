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
  /**
   * Frees the memory of the given IntArray
   */
  free(arr->items);
  free(arr);
}

void print_int_arr(IntArray* arr) {
  /**
   * Prints the contents of the given IntArray to stdout, for debugging
   */
  for (int i = 0; i < arr->size; i++)
    printf("%d ", arr->items[i]);
  printf("\n");
}

void copy_int_arr(IntArray* src, IntArray* dest) {
  /**
   * Copies the contents of the src IntArray to the dest IntArray, does not transfer the amount of unallocated space
   */
  if (dest->total_size < src->size) {
    fprintf(stderr,"dest array's total size %d less than source array size of %d", dest->total_size, src->size);
    exit(EXIT_FAILURE);
  }
  dest->size = src->size;
  memcpy(dest->items, src->items, sizeof(int) * src->size);
}

void add_to_int_arr(IntArray* arr, int item) {
  /**
   * Adds the given integer to the end of the given IntArray, resizing it if out of room
   */
  if (arr->size >= arr->total_size) {
    int new_size = (int) ceil(INT_ARR_RESIZE_CONST * arr->total_size);
    int* temp = calloc(new_size, sizeof(int));
    memcpy(temp, arr->items, sizeof(int) * arr->total_size);
    free(arr->items);
    arr->total_size = new_size;
    arr->items = temp;
  }
  arr->items[arr->size] = item;
  arr->size++;
}

  void write_int_arr(IntArray* arr, FILE* file) {
  /**
   * Writes the contents of the given IntArray to the given FILE stream
   */
  for (int i = 0; i < arr->size; i++) {
    fprintf(file, "%d", arr->items[i]);
    if (i != arr->size - 1)
      fputc(',', file);
  }
}

bool same_int_arr(IntArray* a, IntArray* b) {
  if (a->size != b->size)
    return false;

  for (int i = 0; i < a->size; i++)
    if (a->items[i] != b->items[i])
      return false;

  return true;
}

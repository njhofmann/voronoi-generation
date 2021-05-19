//
// Created by natejh on 5/14/21.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "parse.h"

static const int STARTING_POINT_ARR_SIZE = 3;

int parse_pos_num(char* raw_arg) {
  int val = strtol(raw_arg, NULL, 10);
  if (val <= 0) {
    fprintf(stderr, "positive number must be > 0");
    exit(EXIT_FAILURE);
  }
  return val;
}

int find_next_arg_idx(int start_idx, int argc, char* argv[]) {
  int end_idx = start_idx;
  while (end_idx < argc && argv[end_idx][0] != '-')
    end_idx++;
  return end_idx;
}

IntArray* parse_point(char* raw_point) {
  IntArray* arr = init_int_array(STARTING_POINT_ARR_SIZE);
  char* token = strtok(raw_point, ",");
  while (token != NULL) {
    add_to_int_arr(arr, (int) strtol(token, NULL, 10));
    token = strtok(NULL, ",");
  }
  return arr;
}

IntMatrix* read_starting_centers_file(char* path) {
  FILE* file = fopen(path, "r");
  if (file == NULL) {
    fprintf(stderr, "file %s doesn't exist", path);
    exit(EXIT_FAILURE);
  }

  // number points from first array / line
  int arr_sz = 0;
  char cur_char;
  bool prev_char_int = true;
  cur_char = fgetc(file);
  bool some_input = false;
  while (cur_char != '\n' && cur_char != EOF) {
    some_input = true;
    if (cur_char == ',') {
      if (prev_char_int) {
        fprintf(stderr, "two commas in a row in centers file");
        exit(EXIT_FAILURE);
      }
      arr_sz += 1;
    }
    else {
      prev_char_int = false;
    }
    cur_char = fgetc(file);
  }

  // count last variable
  if (some_input)
    arr_sz++;

  if (arr_sz == 0) {
    fprintf(stderr, "no inputs on first line");
    exit(EXIT_FAILURE);
  }

  rewind(file);

  // read in line by line, convert to int array and add to matrix
  IntMatrix* centers = init_empty_int_matrix(10);
  // TODO make these functions
  char point_holder[20];  // TODO long enough
  while (fscanf(file, "%[^\n] ", &point_holder[0]) != EOF) {
    IntArray* cur_center = init_int_array(arr_sz);

    int idx = 0;
    char* cur_token = strtok(&point_holder[0], ",");
    while (cur_token != NULL) {
      int cur_var = strtol(cur_token, NULL, 0);
      add_to_int_arr(cur_center, cur_var);
      idx++;
      cur_token = strtok(NULL, ",");
    }

    if (idx != arr_sz || cur_center->size != arr_sz) {
      fprintf(stderr, "expected %d points in centers file but got %d", arr_sz, idx);
      exit(EXIT_FAILURE);
    }

    add_int_matrix(centers, cur_center);
  }

  if (fclose(file) != 0) {
    fprintf(stderr, "failed to close file %s", path);
    exit(EXIT_FAILURE);
  }

  return centers;
}

IntMatrix* parse_starting_centers(int start_idx, int argc, char* argv[]) {
  int end_idx = find_next_arg_idx(start_idx, argc, argv);

  if (end_idx - start_idx == 1)
    return read_starting_centers_file(argv[start_idx]);

  int arr_count = end_idx - start_idx;
  IntArray** centers = malloc(sizeof(IntArray*) * arr_count);
  for (int i = 0; i < arr_count; i++)
    centers[i] = parse_point(argv[i + start_idx]);
  return init_int_matrix_from_int_arrs(centers, arr_count);
}

void valid_boundary(IntArray* lower_left, IntArray* upper_right) {
  if (lower_left->size != upper_right->size) {
    fprintf(stderr, "lower left point and upper right point must have same number of axes");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < lower_left->size; i++)
    if (lower_left->items[i] >= upper_right->items[i]) {
      fprintf(stderr, "%dth coodinate in lower left point is greater than upper right points", i);
      exit(EXIT_FAILURE);
    }
}

IntMatrix* parse_boundary(int start_idx, int argc, char* argv[]) {
  int end_idx = find_next_arg_idx(start_idx, argc, argv);
  if (end_idx - start_idx != 2) {
    fprintf(stderr, "expect 2 args for bounding box - lower left and upper right points");
    exit(EXIT_FAILURE);
  }

  IntArray* lower_left = parse_point(argv[start_idx]);
  IntArray* upper_right = parse_point(argv[start_idx + 1]);
  valid_boundary(lower_left, upper_right);

  // free original int arrays, add our own
  // TODO make this a method?
  IntMatrix* boundary = init_int_matrix(lower_left->size, 2);
  free_int_array(boundary->matrix[0]);
  free_int_array(boundary->matrix[1]);
  boundary->matrix[0] = lower_left;
  boundary->matrix[1] = upper_right;
  boundary->width = lower_left->size;  // TODO fix how matrix size works

  return boundary;
}
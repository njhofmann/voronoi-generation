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
  int val = (int)strtol(raw_arg, NULL, 10);
  if (val <= 0) {
    fprintf(stderr, "positive number must be > 0");
    exit(EXIT_FAILURE);
  }
  return val;
}

int parse_int(char* raw_arg) {
  return (int)strtol(raw_arg, NULL, 10); // TODO error checking
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

StartingCentersReturn* parse_starting_centers(int start_idx, int argc, char* argv[]) {
  int end_idx = find_next_arg_idx(start_idx, argc, argv);
  int arg_count = end_idx - start_idx;

  if (arg_count == 0) {
    fprintf(stderr, "need at least one center");
    exit(EXIT_FAILURE);
  }

  StartingCentersReturn* return_centers = malloc(sizeof(StartingCentersReturn));
  return_centers->user_or_rand = true;
  if (arg_count == 1) {
    char* arg = argv[start_idx];
    int count;
    if ((count = (int)strtol(arg, NULL, 0)) != 0) {
      return_centers->centers.random_centers_count = count;
      return_centers->user_or_rand = false;
      return return_centers;
    }
    return_centers->centers.centers = read_starting_centers_file(arg);
    return return_centers;
  }

  IntArray** centers = malloc(sizeof(IntArray*) * arg_count);
  for (int i = 0; i < arg_count; i++)
    centers[i] = parse_point(argv[i + start_idx]);
  return_centers->centers.centers = init_int_matrix_from_int_arrs(centers, arg_count);
  return return_centers;
}
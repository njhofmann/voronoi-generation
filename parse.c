//
// Created by natejh on 5/14/21.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parse.h"

int parse_iterations(char* raw_arg) {
  int val = strtol(raw_arg, NULL, 10);
  if (val < -1) {
    fprintf(stderr, "number of iterations must be >= -1");
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
  IntArray* arr = init_int_array(4);  // TODO get rid of magic number
  char* token = strtok(raw_point, ",");
  while (token != NULL) {
    add_to_int_arr(arr, (int) strtol(token, NULL, 10));
    token = strtok(NULL, ",");
  }
  return arr;
}
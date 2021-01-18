#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "int_array.h"
#include "int_matrix.h"
#include "distance_metric.h"

static struct option LONG_OPTIONS[] = {
    // flag = NULL means val is used to id if arg is included
    {"distance", optional_argument, NULL, 'd'},
    {"centers", required_argument, NULL, 'c'},
    {"box", required_argument, NULL, 'b'},
    {"iterations", optional_argument, NULL, 'i'},
    {"convergence", optional_argument, NULL, 'v'},
    {"output", optional_argument, NULL, 'o'},
    {"full", no_argument, NULL, 'f'},
    {NULL, 0, NULL, 0}
};

static bool FOUND_OPTIONS[7] = {false, false, false, false, false, false, false};

void found_option(int idx, const char* name) {
  if (FOUND_OPTIONS[idx]) {
    fprintf(stderr, "flag for option %s was found twice\n", name);
    exit(EXIT_FAILURE);
  }
}

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
  char* token;
  IntArray* arr = init_int_array(4);  // TODO get rid of magic number
  while ((token = strtok(raw_point, ",")) != NULL) {
    int temp = (int)strtol(token, NULL, 10);
    add_to_int_arr(arr, temp);
  }
  return arr;
}

void valid_boundary(IntArray* lower_left, IntArray* upper_right) {
  if (lower_left->size != upper_right-> size) {
    fprintf(stderr, "lower left point and upper right point must have same number of axes");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < lower_left->size; i++) {
    if (lower_left->items[i] >= upper_right->items[i]) {
      fprintf(stderr, "%dth coodinate in lower left point is greater than upper right points", i);
      exit(EXIT_FAILURE);
    }
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

  return boundary;
}

IntMatrix* read_starting_centers_file(char* path) {
  // TODO me
}

IntMatrix* parse_starting_centers(int start_idx, int argc, char* argv[]) {
  int end_idx = find_next_arg_idx(start_idx, argc, argv);

  if (end_idx - start_idx == 1)
    return read_starting_centers_file(argv[start_idx]);

  IntMatrix* centers = NULL;
  IntArray* temp = NULL;
  for (int i = start_idx; i < end_idx; i++) {
    temp = parse_point(argv[i]);
    if (centers == NULL)
      centers = init_int_matrix(temp->size, 10);
    add_int_matrix(centers, temp);
  }

  return centers;
}

IntMatrix* add_bounds_to_int_array(IntArray* arr, int start, int end) {
  int range_size = end - start + 1;
  IntMatrix* expanded = init_int_matrix_from_int_arr(arr, range_size); // inclusive: [start, end]
  for (int i = 0; i < range_size; i++)
    add_to_int_arr(expanded->matrix[i], start + i);
  // TODO double check matrix bounds here?
  expanded->width = expanded->matrix[0]->size;
  return expanded;
}


IntMatrix* points_in_boundary(IntMatrix* box) {
  // TODO assume only size 2 or check?
  // assume lower left than upper right
  // TODO matrix width = array total_size, not size - how to fix this?
  IntArray* lower_left = box->matrix[0];
  IntArray* upper_right = box-> matrix[1];
  IntMatrix* points = init_int_matrix(box->width, upper_right->items[0] - lower_left->items[0]);

  // assign each item in first dimension (inclusive) to first index of each array in order of appearance
  for (int i = 0; i <= upper_right->items[0] - lower_left->items[0] + 1; i++)
    points->matrix[i]->items[0] = lower_left->items[0] + i;

  // start in second dimension
  for (int dim = 1; dim < box->width; dim++) {

  }
  // for each dimension
  // add each point in current dimension to to a copy of each point in points w/ add_bounds
  // concat each iteration
}


int main(int argc, char* argv[]) {

  if (argc == 1) {
    fprintf(stderr, "require some arguments");
    exit(EXIT_FAILURE);
  }

  // use optind and argc check to parse
  DistanceMetric distance_metric = EUCLIDEAN;
  int iterations = 0;
  bool full_output = false;
  double convergence = .001;
  char* output_path = NULL;
  IntMatrix* starting_centers;
  IntMatrix* bounding_box;

  // short options - : == required, :: = optional
  char cur_arg = 0;
  while ((cur_arg = getopt_long(argc, argv, "d:c::b::i:c:of:", LONG_OPTIONS, NULL)) != -1) {
    switch (cur_arg) {
      case 'd':
        found_option(0, "distance");
        distance_metric = parse_distance_metric(argv[optind]);
        break;
      case 'c':
        found_option(1, "centers");
        starting_centers = parse_starting_centers(optind, argc, argv);
        break;
      case 'b':
        found_option(2, "boundary");
        bounding_box = parse_boundary(optind, argc, argv);
        break;
      case 'i':
        found_option(3, "iterations");
        iterations = parse_iterations(argv[optind]);
        break;
      case 'v':
        found_option(4, "convergence");
        convergence = strtod(argv[optind], NULL);
        break;
      case 'o':
        found_option(5, "output");
        output_path = malloc(strlen(argv[optind]) * sizeof(output_path));
        output_path = strcpy(output_path, argv[optind]);
        break;
      case 'f':
        found_option(6, "full output");
        full_output = true;
        break;
      case '?':
        fprintf(stderr, "unknown arg or arg with a missing required param `-%c`\n", optopt);
        exit(EXIT_FAILURE);
      default:
        fprintf(stderr, "error default case");
        exit(EXIT_FAILURE);
    }
  }

  if (bounding_box->width != starting_centers->width) {
    fprintf(stderr, "number of axes in boundary box does equal number of axes in starting center");
    exit(EXIT_FAILURE);
  }

  free_int_matrix(bounding_box);
  free_int_matrix(starting_centers);
  // TODO generate all points in bounding box
  // TODO voronoi diagram generator
  // TODO output results
  return EXIT_SUCCESS;
}

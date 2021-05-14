#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "int_array.h"
#include "int_matrix.h"
#include "distance_metric.h"
#include "voronoi.h"
#include "parse.h"

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
  if (box->height != 2) {
    fprintf(stderr, "bounding box must have exact two points\n");
    exit(EXIT_FAILURE);
  }

  IntArray* lower_left = box->matrix[0];
  IntArray* upper_right = box->matrix[1];
  int first_dim_size = upper_right->items[0] - lower_left->items[0] + 1;
  IntMatrix* points = init_int_matrix(box->width, first_dim_size);

  // assign each item in first dimension (inclusive) to first index of each array in order of appearance
  for (int i = 0; i < first_dim_size; i++)
    add_to_int_arr(points->matrix[i], lower_left->items[0] + i);

  // start in second dimension
  // for each dimension, add entire range of that dimension to each item in the existing array of points
  for (int dim = 1; dim < box->width; dim++) {
    int matrix_count = points->height;
    IntMatrix* temps[matrix_count];// = malloc(sizeof(IntMatrix*) * matrix_count);
    for (int i = 0; i < points->height; i++)
      temps[i] = add_bounds_to_int_array(points->matrix[i], lower_left->items[dim], upper_right->items[dim]);
    free_int_matrix(points);
    points = concat_int_matrices(temps, matrix_count);
  }

  return points;
}

FILE* check_file_path(char* file_path) {
  FILE* file;
  // "x" forces error if file already exists
  if ((file = fopen(file_path, "wx")) == NULL) {
    fprintf(stderr, "failed to open file at %s\n", file_path);
    exit(EXIT_FAILURE);
  }
  return file;
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
  FILE* output_file = stdout;
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
        printf("%c", argv);
        found_option(3, "iterations");
        iterations = parse_iterations(argv[optind - 1]);
        break;
      case 'v':
        found_option(4, "convergence");
        convergence = strtod(argv[optind], NULL);
        break;
      case 'o':
        found_option(5, "output");
        output_file = check_file_path(argv[optind]);
//        output_file = malloc(strlen(argv[optind]) * sizeof(output_file));
//        output_file = strcpy(output_file, argv[optind]);
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

  IntMatrix* points = points_in_boundary(bounding_box);
  voronoi_relaxation(points, starting_centers, distance_metric, iterations, convergence, output_file);
//  Cells* voronoi_diagram = create_voronoi_diagram(starting_centers, points, distance_metric);
//
//  free_cells(voronoi_diagram); // doesn't free underlying points or centers
  fclose(output_file);
  //free_int_matrix(points);
  free_int_matrix(bounding_box);
  return EXIT_SUCCESS;
}

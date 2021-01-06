#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "int_array.h"
#include "int_matrix.h"

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

typedef enum {
  EUCLIDEAN = 1,
  MANHATTAN = 2,
  MAHALANOBIS = 3,
  RAMANUJAN = 4
} DistanceMetric;

bool streq(char* a, char* b) {
  return strcmp(a, b) == 0;
}

DistanceMetric parse_distance_metric(char* raw_arg) {
  if (streq(raw_arg, "euclidean"))
    return EUCLIDEAN;
  else if (streq(raw_arg, "manhattan"))
    return MANHATTAN;
  else if (streq(raw_arg, "MAHALANOBIS"))
    return MAHALANOBIS;
  else if (streq(raw_arg, "RAMANUJAN"))
    return RAMANUJAN;

  fprintf(stderr, "invalid distance metric %s\n", raw_arg);
  exit(EXIT_FAILURE);
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
  int temp = 0;
  IntArray* arr = init_int_array(3);  // TODO get rid of magic number
  while ((token = strtok(raw_point, ",")) != NULL) {
    temp = strtol(token, NULL, 10);
  }
  // TODO dynamically resize array to get size of array
  return arr;
}

IntMatrix* parse_boundary(int start_idx, int argc, char* argv[]) {
  int end_idx = find_next_arg_idx(start_idx, argc, argv);
  if (end_idx - start_idx != 2) {
    fprintf(stderr, "expect 2 args for bounding box - lower left and upper right points");
    exit(EXIT_FAILURE);
  }

  IntArray* lower_left = parse_point(argv[start_idx]);
  IntArray* upper_right = parse_point(argv[start_idx + 1]);

  if (lower_left->size != upper_right-> size) {
    fprintf(stderr, "lower left point and upper right point must have same number of axes");
    exit(EXIT_FAILURE);
  }

  // TODO turn int arrays into int matrix of size 2


}

IntMatrix* parse_starting_centers(int start_idx, int argc, char* argv[]) {
  // TODO if single arg == path
  // TODO if multiple, try to parse
}


IntMatrix* points_in_boundary(IntMatrix* box) {

}


int main(int argc, char* argv[]) {
  // use optind and argc check to parse
  DistanceMetric distance_metric = EUCLIDEAN;
  int iterations = 0;
  bool full_output;
  double convergence = .001;
  char* output_path = NULL;
  IntMatrix* starting_centers;
  IntMatrix* bounding_box;
  // short options - : == required, :: = optional
  char cur_arg = 0;
  while ((cur_arg = getopt_long(argc, argv, "d:c::b::i:c:of:", LONG_OPTIONS, NULL)) != -1) {
    switch (cur_arg) {
      case 'd':
        distance_metric = parse_distance_metric(argv[optind]);
        break;
      case 'c':
        starting_centers = parse_starting_centers(optind, argc, argv);
        break;
      case 'b':
        bounding_box = parse_boundary(optind, argc, argv);
        break;
      case 'i':
        iterations = parse_iterations(argv[optind]);
        break;
      case 'v':
        convergence = strtod(argv[optind], NULL);
        break;
      case 'o':
        output_path = malloc(strlen(argv[optind]) * sizeof(output_path));
        output_path = strcpy(output_path, argv[optind]);
        break;
      case 'f':
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

  // TODO generate all points in bounding box
  // TODO voronoi diagram generator
  return 0;
}

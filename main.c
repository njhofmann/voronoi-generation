#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "src/int_matrix.h"
#include "src/distance_metric.h"
#include "src/voronoi.h"
#include "src/parse.h"
#include "src/points.h"

static struct option LONG_OPTIONS[] = {
    // flag = NULL means val is used to id if arg is included
    {"distance", required_argument, NULL, 'd'},
    {"centers", required_argument, NULL, 'c'},
    {"box", required_argument, NULL, 'b'},
    {"iterations", required_argument, NULL, 'i'},
    {"convergence", required_argument, NULL, 'v'},
    {"output", required_argument, NULL, 'o'},
    {"full", no_argument, NULL, 'f'},
    {"override", no_argument, NULL, 'r'},
    {NULL, 0, NULL, 0}
};

static bool FOUND_OPTIONS[8] = {false, false, false, false, false, false, false, false};

void found_option(int idx, const char* name) {
  if (FOUND_OPTIONS[idx]) {
    fprintf(stderr, "flag for option %s was found twice\n", name);
    exit(EXIT_FAILURE);
  }
  FOUND_OPTIONS[idx] = true;
}

FILE* check_file_path(char* file_path, bool override) {
  /**
   * Throws and error if the file at the give file path exists, else returns a FILE* to that file
   */
  FILE* file;
  // "x" forces error if file already exists
  if (override) {
    file = fopen(file_path, "w");
  }
  else if ((file = fopen(file_path, "wx")) == NULL) {
    fprintf(stderr, "failed to open file at %s, file already exists\n", file_path);
    exit(EXIT_FAILURE);
  }
  return file;
}

int main(int argc, char* argv[]) {
  if (argc == 1) {
    fprintf(stderr, "require some arguments\n");
    exit(EXIT_FAILURE);
  }

  // use optind and argc check to parse
  DistanceMetric distance_metric = EUCLIDEAN;
  int iterations = -1;
  bool full_output = false;
  double convergence = 1;
  FILE* output_file = stdout;
  char* output_file_path; // TODO redo this
  bool override_results = false;
  IntMatrix* starting_centers;
  IntMatrix* bounding_box;

  // short options - : means required, :: means optional
  char cur_arg;
  while ((cur_arg = getopt_long(argc, argv, "d:c:b:i:v:o:fr", LONG_OPTIONS, NULL)) != -1) {
    switch (cur_arg) {
      case 'd':
        found_option(0, "distance");
        distance_metric = parse_distance_metric(argv[optind-1]);
        break;
      case 'c':
        found_option(1, "centers");
        starting_centers = parse_starting_centers(optind-1, argc, argv);
        break;
      case 'b':
        found_option(2, "boundary");
        bounding_box = parse_boundary(optind-1, argc, argv);
        break;
      case 'i':
        found_option(3, "iterations");
        iterations = parse_iterations(argv[optind - 1]);
        break;
      case 'v':
        found_option(4, "convergence");
        convergence = strtod(argv[optind-1], NULL);
        break;
      case 'o':
        found_option(5, "output");
        output_file_path = argv[optind-1];
        break;
      case 'f':
        found_option(6, "full output");
        full_output = true;
        break;
      case 'r':
        found_option(7, "override");
        override_results = true;
        break;
      case '?':
        fprintf(stderr, "unknown arg or arg with a missing required param `-%c`\n", optopt);
        exit(EXIT_FAILURE);
      default:
        fprintf(stderr, "error default case");
        exit(EXIT_FAILURE);
    }
  }

  if (!FOUND_OPTIONS[1]) {
    fprintf(stderr, "require starting centers");
    exit(EXIT_FAILURE);
  }

  if (!FOUND_OPTIONS[2]) {
    fprintf(stderr, "require bounding box");
    exit(EXIT_FAILURE);
  }

  if (FOUND_OPTIONS[6])
    output_file = check_file_path(output_file_path, override_results);

  if (convergence < 0 && iterations < 0) {
    fprintf(stderr, "need at least a positive convergence threshold or positive number of iterations");
    exit(EXIT_FAILURE);
  }

  if (bounding_box->width != starting_centers->width) {
    fprintf(stderr, "number of axes in boundary box does equal number of axes in starting center");
    exit(EXIT_FAILURE);
  }

  IntMatrix* points = get_points_in_bounding_box(bounding_box);
  free_int_matrix(bounding_box);

  voronoi_relaxation(points, starting_centers, distance_metric, iterations, convergence, output_file, full_output);
  fclose(output_file);
  return EXIT_SUCCESS;
}

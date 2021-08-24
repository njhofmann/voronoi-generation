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
#include "src/file_utils.h"

static struct option LONG_OPTIONS[] = {
    // flag = NULL means val is used to id if arg is included
    {"distance", required_argument, NULL, 'd'},
    {"centers", required_argument, NULL, 'c'},
    {"box", required_argument, NULL, 'b'},
    {"iterations", required_argument, NULL, 'i'},
    {"convergence", required_argument, NULL, 'v'},
    {"output_dirc", required_argument, NULL, 'o'},
    {"p", required_argument, NULL, 'p'},
    {"full", no_argument, NULL, 'f'},
    {"override", no_argument, NULL, 'r'},
    {"processes", required_argument, NULL, 'm'},
    {NULL, 0, NULL, 0}
};

static bool FOUND_OPTIONS[10] = {false, false, false, false, false, false, false, false, false, false};

void found_option(int idx, const char* name) {
  if (FOUND_OPTIONS[idx]) {
    fprintf(stderr, "flag for option %s was already included\n", name);
    exit(EXIT_FAILURE);
  }
  FOUND_OPTIONS[idx] = true;
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
  double convergence = -1;
  char* output_dirc = NULL;
  bool override_results = false;
  int p = 2;
  int num_of_processes = 1;
  StartingCentersReturn* starting_centers_return;
  IntArray* bounding_box;

  // short options - : means required, :: means optional
  char cur_arg;
  while ((cur_arg = (char) getopt_long(argc, argv, "d:c:b:i:v:o:frp:c:m:", LONG_OPTIONS, NULL)) != -1) {
    switch (cur_arg) {
      case 'd':
        found_option(0, "distance");
        distance_metric = parse_distance_metric(argv[optind - 1]);
        break;
      case 'c':
        found_option(1, "centers");
        starting_centers_return = parse_starting_centers(optind - 1, argc, argv);
        break;
      case 'b':
        found_option(2, "boundary");
        bounding_box = parse_point(argv[optind - 1]);
        break;
      case 'i':
        found_option(3, "iterations");
        iterations = parse_pos_num(argv[optind - 1]);
        break;
      case 'v':
        found_option(4, "convergence");
        convergence = strtod(argv[optind - 1], NULL);
        break;
      case 'o':
        found_option(5, "output directory");
        output_dirc = argv[optind - 1];
        break;
      case 'f':
        found_option(6, "full output");
        full_output = true;
        break;
      case 'p':
        found_option(7, "p");
        p = parse_pos_num(argv[optind - 1]);
        break;
      case 'r':
        found_option(8, "override");
        override_results = true;
        break;
      case 'm':
        found_option(9, "multiple processes");
        num_of_processes = parse_pos_num(argv[optind - 1]);
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

  if (FOUND_OPTIONS[5])
    create_dirc(output_dirc, override_results);

  if (convergence < 0 && iterations < 0) {
    fprintf(stderr, "need a positive convergence threshold and / or positive number of iterations");
    exit(EXIT_FAILURE);
  }

  IntMatrix* starting_centers = starting_centers_return->user_or_rand ?
      starting_centers_return->centers.centers :
      random_points(starting_centers_return->centers.random_centers_count, bounding_box);

  print_int_matrix(starting_centers);
  if (bounding_box->size != starting_centers->width) {
    fprintf(stderr, "number of axes in boundary box does equal number of axes in starting center");
    exit(EXIT_FAILURE);
  }

  int box_size = num_of_points(bounding_box);
  if (box_size < starting_centers->height) {
    fprintf(stderr, "%d points in bounding box and %d starting centers, former must be > than the latter",
            box_size, starting_centers->height);
    exit(EXIT_FAILURE);
  }

  // can't have empty processes
  if (num_of_processes > starting_centers->height)
    num_of_processes = starting_centers->height;

  valid_centers(bounding_box, starting_centers);

  IntMatrix* points = get_points_in_bounding_box(bounding_box);
  voronoi_relaxation(bounding_box,
                     points,
                     starting_centers,
                     distance_metric,
                     iterations,
                     convergence,
                     output_dirc,
                     full_output,
                     num_of_processes,
                     p);

  free(starting_centers_return);
  free_int_array(bounding_box);
  free_int_matrix(points);
  return EXIT_SUCCESS;
}

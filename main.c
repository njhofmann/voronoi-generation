#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

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
} distance_metric;

bool streq(char* a, char* b) {
  return strcmp(a, b) == 0
}

distance_metric parse_distance_metric(char* raw_arg) {
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

int main(int argc, char* argv[]) {
  // use optind and argc check to parse
  int distance_metric = 1;
  int iterations = 0;
  bool full_output;
  double convergence = .001;
  char* output_path = NULL;
//  int* starting_centers[];
//  int* bounding_box[] = malloc;
  // short options - : == required, :: = optional
  char cur_arg = 0;
  while ((cur_arg = getopt_long(argc, argv, "d:c::b::i:c:of:", LONG_OPTIONS, NULL)) != -1) {
    switch (cur_arg) {
      case 'd':
        distance_metric = parse_distance_metric(argv[optind]);
        break;
      case 'c':
        break;
      case 'b':
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
        // TODO invalid case
        return -1;
    }
  }
  return 0;
}

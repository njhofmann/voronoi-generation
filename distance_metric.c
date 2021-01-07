//
// Created by natejh on 1/6/21.
//

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "distance_metric.h"

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

double compute_distance_metric(IntArray* a, IntArray* b, DistanceMetric metric) {

}
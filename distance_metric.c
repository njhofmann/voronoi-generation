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

double manhattan_dist(IntArray* a, IntArray* b) {
  int sum = 0;
  for (int i = 0; i < a->size; i++)
    sum += abs(a->items[i] - b->items[i]);
  return sum;
}

double mean_absolute_error_dist(IntArray* a, IntArray* b) {
  return manhattan_dist(a, b) / a->size;
}

double chebyshev_dist(IntArray* a, IntArray* b) {
  int max = 0;
  for (int i = 0; i < a->size; i++) {
    int temp = abs(a->items[i] - b->items[i]);
    if (i == 0 || temp > max) {
      max = temp;
    }
  }
  return max;
}

void invalid_distance_metric() {
  fprintf(stderr, "invalid distance metric\n");
  exit(EXIT_FAILURE);
}

DistanceMetric parse_distance_metric(char* raw_arg) {
  if (streq(raw_arg, "euclidean"))
    return EUCLIDEAN;
  else if (streq(raw_arg, "manhattan"))
    return MANHATTAN;
  else if (streq(raw_arg, "mahalanobis"))
    return MAHALANOBIS;
  else if (streq(raw_arg, "ramanujan"))
    return RAMANUJAN;
  else if (streq(raw_arg, "chebyshev"))
    return CHEBYSHEV;
  else if (streq(raw_arg, "canberra"))
    return CANBERRA;
  else if (streq(raw_arg, "pearson"))
    return PEARSON;
  else if (streq(raw_arg, "cosine"))
    return COSINE;
  else if (streq(raw_arg, "mse"))
    return MSE;
  else if (streq(raw_arg, "mae"))
    return MAE;
  else if (streq(raw_arg, "ssd"))
    return SSD;

  invalid_distance_metric();
}

double compute_distance_metric(IntArray* a, IntArray* b, DistanceMetric metric) {
  switch (metric) {
    case EUCLIDEAN:
      break;
    case MANHATTAN:
      return manhattan_dist(a, b);
    case MAHALANOBIS:
      break;
    case RAMANUJAN:
      break;
    case CHEBYSHEV:
      return chebyshev_dist(a, b);
    case CANBERRA:
      break;
    case PEARSON:
      break;
    case COSINE:
      break;
    case MSE:
      break;
    case MAE:
      return mean_absolute_error_dist(a, b);
    case SSD:
      break;
    default:
      invalid_distance_metric();
  }
}
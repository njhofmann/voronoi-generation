//
// Created by natejh on 1/6/21.
//

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "distance_metric.h"

bool streq(char* a, char* b) {
  return strcmp(a, b) == 0;
}

double minkowski_dist(IntArray* a, IntArray* b, int p) {
  double sum = 0.0;
  for (int i = 0; i < a->size; i++)
    sum += pow(abs(a->items[i] - b->items[i]), p);
  return pow(sum, 1.0 / (p * 1.0));
}

double manhattan_dist(IntArray* a, IntArray* b, int p) {
  return minkowski_dist(a, b, 1);
}

double chebyshev_dist(IntArray* a, IntArray* b, int p) {
  int max = 0;
  for (int i = 0; i < a->size; i++) {
    int temp = abs(a->items[i] - b->items[i]);
    if (i == 0 || temp > max)
      max = temp;
  }
  return max;
}

double euclidean_dist(IntArray* a, IntArray* b, int p) {
  return minkowski_dist(a, b, 2);
}

double canberra_dist(IntArray* a, IntArray* b, int p) {
  double sum = 0;
  for (int i = 0; i < a->size; i++)
    sum += abs(a->items[i] - b->items[i]) / (1.0 * (abs(a->items[i]) + abs(b->items[i])));
  return sum;
}

double bray_curtis_dist(IntArray* a, IntArray* b, int p) {
  double numerator = 0;
  double denominator = 0;
  for (int i = 0; i < a->size; i++) {
    numerator += abs(a->items[i] - b->items[i]);
    denominator += abs(a->items[i] + b->items[i]);  // TODO is this wrong?
  }

  if (denominator < 0.00001)
    return 0;
  return numerator / denominator;
}

double yang_dist(IntArray* a, IntArray* b, int p) {
  double x_greater = 0.0;
  double x_lesser = 0.0;
  for (int i = 0; i < a->size; i++) {

    int x = a->items[i];
    int y = b->items[i];
    if (x < y)
      x_lesser += (y - x);
    else
      x_greater += (x - y);
  }
  return pow(pow(x_greater, p) + pow(x_lesser, p), 1.0 / (1.0 * p));
}

double hellinger_dist(IntArray* a, IntArray* b, int p) {
  double t = 0;
  for (int i = 0; i < a->size; i++)
    t += pow(sqrt(a->items[i]) - sqrt(b->items[i]), 2.0);
  return (1.0 / sqrt(2.0)) * sqrt(t);
}

/**
 * Array of all distance metric methods
 */
double (*dist_func[8])(IntArray*, IntArray*, int) = {
    euclidean_dist,
    manhattan_dist,
    chebyshev_dist,
    canberra_dist,
    bray_curtis_dist,
    minkowski_dist,
    yang_dist,
    hellinger_dist
};

DistanceMetric parse_distance_metric(char* raw_arg) {
  if (streq(raw_arg, "euclidean"))
    return EUCLIDEAN;
  else if (streq(raw_arg, "manhattan"))
    return MANHATTAN;
  else if (streq(raw_arg, "chebyshev"))
    return CHEBYSHEV;
  else if (streq(raw_arg, "canberra"))
    return CANBERRA;
  else if (streq(raw_arg, "bray-curtis"))
    return BRAY_CURTIS;
  else if (streq(raw_arg, "minkowski"))
    return MINKOWSKI;
  else if (streq(raw_arg, "yang"))
    return YANG;
  else if (streq(raw_arg, "hellinger"))
    return HELLINGER;

  fprintf(stderr, "%s is an invalid distance metric\n", raw_arg);
  exit(EXIT_FAILURE);
}

double compute_distance_metric(IntArray* a, IntArray* b, DistanceMetric metric, int p) {
  if (metric > 7 || metric < 0) {
    fprintf(stderr, "unsupported distance metric\n");
    exit(EXIT_FAILURE);
  }
  return (*dist_func[metric])(a, b, p);
}
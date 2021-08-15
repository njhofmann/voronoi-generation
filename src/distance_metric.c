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

double average(IntArray* a) {
  double sum = 0;
  for (int i = 0; i < a->size; i++)
    sum += a->items[i];
  return sum / a->size;
}

double pearsons_dist_help(IntArray* a, double avg) {
  double sum = 0;
  for (int i = 0; i < a->size; i++)
    sum += pow(a->items[i], 2);
  return sqrt(sum - (a->size * pow(avg, 2)));
}

double pearsons_dist(IntArray* a, IntArray* b, int p) {
  // https://en.wikipedia.org/wiki/Pearson_correlation_coefficient#For_a_sample
  double a_avg = average(a);
  double b_avg = average(b);
  double a_dem = pearsons_dist_help(a, a_avg);
  double b_dem = pearsons_dist_help(b, b_avg);

  double sum = 0;
  for (int i = 0; i < a->size; i++)
    sum += a->items[i] * b->items[i] * 1.0;

  return 1 - ((sum  - (a->size * a_avg * b_avg)) / (a_dem * b_dem));
}

double squared_sum(IntArray* a) {
  double sum = 0;
  for (int i = 0; i < a->size; i++)
    sum += 1.0 * pow(a->items[i], 2);
  return sum;
}

double ramanujan_dist(IntArray* a, IntArray* b, int p) {
  // https://kenta.blogspot.com/2004/08/ramanujan-and-ellipses.html
  double sum = 0;
  for (int i = 0; i < a->size; i++) {
    int x = a->items[i];
    int y = b->items[i];
    double h = pow((x - y) / (1.0 * (x + y)), 2);
    sum += (3.1415926 * (x + y)) * ((1 + (3 * h)) / (10 + sqrt(4 - (3 * h))));
  }
  return sum;
}

double cosine_dist(IntArray* a, IntArray* b, int p) {
  double sum = 0;
  for (int i = 0; i < a->size; i++)
    sum += 1.0 * a->items[i] * b->items[i];
  return sum / (squared_sum(a) * squared_sum(b));
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
  return pow(pow(x_greater, p) + pow(x_lesser, p) , 1.0 / (1.0 * p));
}

/**
 * Array of all distance metric methods
 */
double (*dist_func[10])(IntArray*, IntArray*, int) = {
    euclidean_dist,
    manhattan_dist,
    ramanujan_dist,
    chebyshev_dist,
    canberra_dist,
    pearsons_dist,
    cosine_dist,
    bray_curtis_dist,
    minkowski_dist,
    yang_dist
};

DistanceMetric parse_distance_metric(char* raw_arg) {
  if (streq(raw_arg, "euclidean"))
    return EUCLIDEAN;
  else if (streq(raw_arg, "manhattan"))
    return MANHATTAN;
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
  else if (streq(raw_arg, "bray-curtis"))
    return BRAY_CURTIS;
  else if (streq(raw_arg, "minkowski"))
    return MINKOWSKI;
  else if (streq(raw_arg, "yang"))
    return YANG;

  fprintf(stderr, "%s is an invalid distance metric\n", raw_arg);
  exit(EXIT_FAILURE);
}

double compute_distance_metric(IntArray* a, IntArray* b, DistanceMetric metric, int p) {
  if (metric > 9 || metric < 0) {
    fprintf(stderr, "unsupported distance metric\n");
    exit(EXIT_FAILURE);
  }
  return (*dist_func[metric])(a, b, p);
}
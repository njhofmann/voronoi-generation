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

double sum_of_squared_diff(IntArray* a, IntArray* b) {
  double sum = 0;
  for (int i = 0; i < a->size; i++)
    sum += pow(a->items[i] - b->items[i], 2);
  return sum;
}

double mean_squared_error(IntArray* a, IntArray* b) {
  return sum_of_squared_diff(a, b) / a->size;
}

double euclidean_dist(IntArray* a, IntArray* b) {
  return sqrt(sum_of_squared_diff(a, b));
}

double canberra_dist(IntArray* a, IntArray* b) {
  double sum = 0;
  for (int i = 0; i < a->size; i++)
    sum += abs(a->items[i] - b->items[i]) / (1.0 * (abs(a->items[i] + abs(b->items[i]))));
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

double pearsons_dist(IntArray* a, IntArray* b) {
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

double ramanujan_dist(IntArray* a, IntArray* b) {
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

double cosine_dist(IntArray* a, IntArray* b) {
  double sum = 0;
  for (int i = 0; i < a->size; i++)
    sum += 1.0 * a->items[i] * b->items[i];
  return sum / (squared_sum(a) * squared_sum(b));
}

double (*dist_func[10])(IntArray*, IntArray*) = {
    euclidean_dist,
    manhattan_dist,
    ramanujan_dist,
    chebyshev_dist,
    canberra_dist,
    pearsons_dist,
    cosine_dist,
    mean_squared_error,
    mean_absolute_error_dist,
    sum_of_squared_diff
};

void invalid_distance_metric() {
  fprintf(stderr, "invalid distance metric\n");
  exit(EXIT_FAILURE);
}

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
  else if (streq(raw_arg, "mse"))
    return MSE;
  else if (streq(raw_arg, "mae"))
    return MAE;
  else if (streq(raw_arg, "ssd"))
    return SSD;

  invalid_distance_metric();
}

double compute_distance_metric(IntArray* a, IntArray* b, DistanceMetric metric) {
  if (metric > 10) {
    fprintf(stderr, "unsupported distance metric\n");
    exit(EXIT_FAILURE);
  }
  return (*dist_func[metric])(a, b);
}
//
// Created by natejh on 8/24/21.
//

#include <stdlib.h>
#include <stdio.h>
#include "closest_center.h"
#include "int_array.h"
#include "int_matrix.h"
#include "distance_metric.h"

typedef struct {
  double dist;
  int idx;
} Dist;

void swap(Dist dists[], int a, int b) {
  Dist temp = dists[a];
  dists[a] = dists[b];
  dists[b] = temp;
}

void print_dists(Dist dists[], int size) {
  for (int i = 0; i < size; i++)
    printf("%d ", dists[i].idx);
  puts("");
}

int partition(Dist dists[], int left, int right, int pivot_idx) {
  Dist pivot = dists[pivot_idx];
  swap(dists, pivot_idx, right);
  pivot_idx = left;
  for (int i = left; i < right; i++)
    if (dists[i].dist <= pivot.dist) {
      swap(dists, i, pivot_idx);
      pivot_idx++;
    }

  swap(dists, pivot_idx, right);
  return pivot_idx;
}

int quickselect(Dist dists[], int left, int right, int k) {
  if (left == right)
    return dists[left].idx;

  int pivot = left + rand() % (right - left + 1);
  pivot = partition(dists, left, right, pivot);

  if (k == pivot)
    return dists[k].idx;
  else if (k < pivot)
    return quickselect(dists, left, pivot - 1, k);
  return quickselect(dists, pivot + 1, right, k);
}

int find_k_th_closest_center(IntArray* point, IntMatrix* centers, DistanceMetric distance_metric, int p, int k) {
  Dist dists[centers->height];
  for (int i = 0; i < centers->height; i++) {
    dists[i].dist = compute_distance_metric(point, centers->matrix[i], distance_metric, p);
    dists[i].idx = i;
  }
  return quickselect(dists, 0, centers->height - 1, k);
}

int k_th_closest_center(IntArray* point, IntMatrix* centers, DistanceMetric distance_metric, int p, int k) {
  if (k == 0) {
    int closest_idx = -1;
    double closest_dist = 0.0;
    for (int i = 0; i < centers->height; i++) {
      if (same_int_arr(point, centers->matrix[i]))
        return i;

      double cur_dist = compute_distance_metric(point, centers->matrix[i], distance_metric, p);
      if (closest_idx < 0 || closest_dist > cur_dist) {
        closest_idx = i;
        closest_dist = cur_dist;
      }
    }
    return closest_idx;
  }
  return find_k_th_closest_center(point, centers, distance_metric, p, k);
}

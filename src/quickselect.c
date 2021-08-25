//
// Created by natejh on 8/24/21.
//

#include <stdlib.h>
#include <stdio.h>
#include "quickselect.h"

void swap(Dist dists[], int a, int b) {
  Dist temp = dists[a];
  dists[a] = dists[b];
  dists[b] = temp;
}

void print_dists(Dist dists[], int size) {
  for (int i = 0; i < size; i++)
    printf("%d ", dists[i]);
  puts("");
}

int partition(Dist dists[], int left, int right, int pivot_idx) {
  Dist pivot = dists[pivot_idx];
  //print_dists(dists, right+1);
  swap(dists, pivot_idx, right);
  //print_dists(dists, right+1);
  pivot_idx = left;
  for (int i = left; i < right; i++)
    if (dists[i].dist <= pivot.dist) {
      swap(dists, i, pivot_idx);
      pivot_idx++;
    }

  swap(dists, pivot_idx, right);
  return pivot_idx;
}

int quickselect_in_place(Dist dists[], int left, int right, int k) {
  if (left == right)
    return dists[left].idx;

  int pivot = left + rand() % (right - left + 1);
  pivot = partition(dists, left, right, pivot);

  if (k == pivot)
    return dists[k].idx;
  else if (k < pivot)
    return quickselect_in_place(dists, left, pivot - 1, k);
  return quickselect_in_place(dists, pivot + 1, right, k);
}

int quickselect(Dist dists[], int size, int k) {
  return quickselect_in_place(dists, 0, size - 1, k);
}

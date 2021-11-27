//
// Created by natejh on 8/9/21.
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "util.h"

int* split_array(int array_length, int chunks) {
  int* chunk_idxs = malloc(sizeof(int) * chunks);
  int chunk_size = ceil((1.0 * array_length) / chunks);
  int i = 0;
  int cur_idx = 0;
  do {
    chunk_idxs[i] = cur_idx;
    cur_idx += chunk_size;
    i++;
  } while (i < chunks);

  return chunk_idxs;
}

void set_random_seed(unsigned int seed) {
  srand(seed);
}
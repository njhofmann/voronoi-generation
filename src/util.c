//
// Created by natejh on 8/9/21.
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "util.h"

int* split_array(int array_length, int chunks) {
  /**
   * Returns an array of indices for splitting up an array of the given length into `chunk` number of evenly sized
   * subarrays, i-th index gives the starting index of the i-th chunk
   *
   * If the size of each chunk does not evenly factor into the array length, size of each chunk will be rounded up and
   * the last chunk will be smaller than the earlier chunks
   *
   * Ex:) array of size 10 into two chunks --> {0, 5}
   *      10, 3 --> {0, 4, 8}
   */
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
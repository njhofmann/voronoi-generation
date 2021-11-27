//
// Created by natejh on 8/9/21.
//

#ifndef VORONOI_GENERATION_SRC_UTIL_H_
#define VORONOI_GENERATION_SRC_UTIL_H_

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
int* split_array(int array_length, int chunks);

void set_random_seed(unsigned int seed);

#endif //VORONOI_GENERATION_SRC_UTIL_H_

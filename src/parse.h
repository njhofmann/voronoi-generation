//
// Created by natejh on 5/14/21.
//

#ifndef VORONOI_GENERATION__PARSE_H_
#define VORONOI_GENERATION__PARSE_H_

#include "int_matrix.h"

/**
 * Parsing methods for processing input
 */

typedef struct {
  union {
    IntMatrix* centers;       // user provided centers
    int random_centers_count; // # of random centers to generate
  } centers;
  bool user_or_rand;  // are centers user created or randomly created
} StartingCentersReturn;

int parse_pos_num(char* raw_arg);
int find_next_arg_idx(int start_idx, int argc, char* argv[]);
StartingCentersReturn* parse_starting_centers(int start_idx, int argc, char* argv[]);
IntArray* parse_point(char* raw_point);

#endif //VORONOI_GENERATION__PARSE_H_

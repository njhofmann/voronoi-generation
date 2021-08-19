//
// Created by natejh on 5/14/21.
//

#ifndef VORONOI_GENERATION__PARSE_H_
#define VORONOI_GENERATION__PARSE_H_

#include "int_matrix.h"

/**
 * Parsing methods for processing input
 */

int parse_pos_num(char* raw_arg);
int find_next_arg_idx(int start_idx, int argc, char* argv[]);
IntMatrix* parse_starting_centers(int start_idx, int argc, char* argv[]);
IntArray* parse_point(char* raw_point);

#endif //VORONOI_GENERATION__PARSE_H_

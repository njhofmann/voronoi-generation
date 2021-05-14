//
// Created by natejh on 5/14/21.
//

#ifndef VORONOI_GENERATION__PARSE_H_
#define VORONOI_GENERATION__PARSE_H_

#include "int_array.h"

int parse_iterations(char* raw_arg);
int find_next_arg_idx(int start_idx, int argc, char* argv[]);
IntArray* parse_point(char* raw_point);

#endif //VORONOI_GENERATION__PARSE_H_

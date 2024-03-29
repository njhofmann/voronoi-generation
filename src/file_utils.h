//
// Created by natejh on 5/18/21.
//

#ifndef VORONOI_GENERATION_SRC_FILE_UTILS_H_
#define VORONOI_GENERATION_SRC_FILE_UTILS_H_

#include <stdbool.h>

/**
 * Creates a directory at the given path, with option to override if a directory already exists there
 */
void create_dirc(char* path, bool exists);

#endif //VORONOI_GENERATION_SRC_FILE_UTILS_H_

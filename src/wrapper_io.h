//
// Created by natejh on 8/9/21.
//

#ifndef VORONOI_GENERATION_SRC_WRAPPER_IO_H_
#define VORONOI_GENERATION_SRC_WRAPPER_IO_H_

#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <error.h>

/**
 * Write `count` bytes of the given buffer to the given file descriptor
 */
void exact_write(int fd, const void* buf, size_t count);

/**
 * Read `count` bytes of the given buffer to the given file descriptor
 */
void exact_read(int fd, void* buf, size_t count);

#endif //VORONOI_GENERATION_SRC_WRAPPER_IO_H_

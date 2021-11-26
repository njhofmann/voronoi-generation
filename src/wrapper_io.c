//
// Created by natejh on 8/9/21.
//

#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <error.h>
#include "wrapper_io.h"

void exact_write(int fd, const void* buf, size_t count) {
  size_t r = write(fd, buf, count);
  if (r == -1) {
    fprintf(stderr, "failed to write data\n");
    fprintf(stderr, "errno set to %d\n", errno);
    exit(1);
  }
  else if (r != count) {
    fprintf(stderr, "failed to write all data, expected %zu but wrote %zu\n", count, r);
    fprintf(stderr, "errno set to %d\n", errno);
    exit(1);
  }
}

void exact_read(int fd, void* buf, size_t count) {
  size_t r = read(fd, buf, count);
  if (r == -1) {
    fprintf(stderr, "failed to read data\n");
    fprintf(stderr, "errno set to %d\n", errno);
    exit(1);
  }
  else if (r != count) {
    fprintf(stderr, "failed to read all data, expected %zu but wrote %zu\n", count, r);
    exit(1);
  }
}
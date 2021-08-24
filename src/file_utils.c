//
// Created by natejh on 5/18/21.
//

#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include "file_utils.h"

void create_dirc(char* path, bool exists) {
  if (mkdir(path, S_IRWXU) == -1) {
    switch (errno) {
      case EACCES :
        perror("the parent directory does not allow write\n");
        exit(EXIT_FAILURE);
      case EEXIST:
        if (exists)
          break;
        perror("pathname already exists\n");
        exit(EXIT_FAILURE);
      case ENAMETOOLONG:
        printf("pathname is too long\n");
        exit(EXIT_FAILURE);
      default:
        fprintf(stderr, "failed to make directory %s\n", path);
        exit(EXIT_FAILURE);
    }
  }
}
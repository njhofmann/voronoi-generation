//
// Created by natejh on 8/15/21.
//

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "pipes.h"

int** init_child_pipes(int cnt) {
  int** pipes = malloc(sizeof(int*) * cnt);
  for (int i = 0; i < cnt; i++) {
    pipes[i] = malloc(sizeof(int) * 2);
    if (pipe(pipes[i]) == -1) {
      fprintf(stderr, "failed to create child pipes");
      exit(EXIT_FAILURE);
    }
  }
  return pipes;
}

void free_child_pipes(int** pipes, int cnt) {
  for (int i = 0; i < cnt; i++)
    free(pipes[i]);
  free(pipes);
}

void close_all_other_pipes(int** pipes, int pipe_cnt, int i) {
  for (int j = 0; j < pipe_cnt; j++) {
    close(pipes[j][0]);
    if (i != j)
      close(pipes[j][1]);
  }
}
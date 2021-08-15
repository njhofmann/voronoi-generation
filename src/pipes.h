//
// Created by natejh on 8/15/21.
//

#ifndef VORONOI_GENERATION_SRC_PIPES_H_
#define VORONOI_GENERATION_SRC_PIPES_H_

/**
 * Initiates `cnt` set of pipes
 */
int** init_pipes(int cnt);

/**
 * Frees the given set of `cnt` pipes
 */
void pipes(int** pipes, int cnt);

/**
 * Given a set of pipes, closes every reading and writing pipe, except for the i-th writing pipe. Meant for use in an
 * "i-th" child process
 */
void close_all_other_pipes(int** pipes, int pipe_cnt, int i);

#endif //VORONOI_GENERATION_SRC_PIPES_H_

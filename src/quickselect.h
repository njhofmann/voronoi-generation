//
// Created by natejh on 8/24/21.
//

#ifndef VORONOI_GENERATION_SRC_QUICKSELECT_H_
#define VORONOI_GENERATION_SRC_QUICKSELECT_H_

typedef struct {
  double dist;
  int idx;
} Dist;


int quickselect(Dist dists[], int size, int k);

#endif //VORONOI_GENERATION_SRC_QUICKSELECT_H_

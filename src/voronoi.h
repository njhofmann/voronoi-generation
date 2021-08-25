//
// Created by natejh on 1/6/21.
//

#ifndef VORONOI_GENERATION__VORONOI_H_
#define VORONOI_GENERATION__VORONOI_H_

#include <stdio.h>
#include "int_matrix.h"
#include "distance_metric.h"

/**
 * Executes iterations of Voronoi relaxation from the given set of starting points and centers, using the given
 * DistanceMetric
 *
 * Results are writen to the given FILE stream, if `full_output` is true each iteration is writen
 *
 * If `iterations` is positive, computes that many iterations of relaxation
 *
 * If `converge_threshold` is positive, executes iterations until the center of some cluster moves <
 * `converge_theshold` between iterations
 *
 * If `iterations` and `converge_threshold` used, stops on the first one to be met
 */
void voronoi_relaxation(IntArray* dimensions, IntMatrix* points, IntMatrix* centers, DistanceMetric metric,
                        int iterations, double converge_threshold, char* output_dirc, bool full_output,
                        int process_cnt, int p, int k);

#endif //VORONOI_GENERATION__VORONOI_H_

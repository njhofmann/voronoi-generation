cmake_minimum_required(VERSION 3.17)
project(voronoi_generation C)

set(CMAKE_C_STANDARD 11)

add_executable(voronoi_generation main.c int_array.h int_array.c int_matrix.c int_matrix.h voronoi.c voronoi.h distance_metric.c distance_metric.h parse.h parse.c)
target_link_libraries(voronoi_generation m)
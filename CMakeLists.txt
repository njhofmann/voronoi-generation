cmake_minimum_required(VERSION 3.17)
project(voronoi_generation C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
message(${CMAKE_C_FLAGS})

add_executable(voronoi_generation main.c src/int_array.h src/int_array.c src/int_matrix.c src/int_matrix.h src/voronoi.c src/voronoi.h src/distance_metric.c src/distance_metric.h src/parse.h src/parse.c src/points.c src/points.h src/int_tensor.c src/int_tensor.h src/file_utils.c src/file_utils.h src/cells.c src/cells.h src/wrapper_io.c src/wrapper_io.h src/util.c src/util.h src/pipes.c src/pipes.h src/closest_center.c src/closest_center.h)
target_link_libraries(voronoi_generation m)
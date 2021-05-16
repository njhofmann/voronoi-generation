# voronoi-generation
Point-by-point, *n*-dimensional Voronoi diagram generation and relaxation in C.

<p align="center">
  <img width="600" src="https://media3.giphy.com/media/9FaM4l3F6FLsjvatmk/giphy.gif?cid=790b761167f73c5c8d9807451e5d20ddfc6e121b299f8d03&rid=giphy.gif&ct=g" alt="header">
</p>

## Background 

This is small C program for [Voronoi diagram](https://en.wikipedia.org/wiki/Voronoi_diagram) and [relaxation](https://en.wikipedia.org/wiki/Lloyd%27s_algorithm) under a variety of distance functions.

Instead of utilizing algorithms like [Fortune's algorithm](https://en.wikipedia.org/wiki/Fortune%27s_algorithm) that use geometric structures, this implementation opts for a point-by-point implementation - i.e. using points like (1, 1) and (2, 2) directly. The resulting algorithm is ~*n*^*k* in runtime, where n is the space within each dimension and k is the number of dimensions), but is vastly simpler in its implementation and output.

## Usage

This project uses CMake to build the program, producing the executable `voronoi_generation`.

The executable accepts the following arguments in any order:

- `--distance [-d] {optional, default: euclidian}`: distance metric to use; one of the following ...

- `--centers [-c] {required}`: starting center points, either listed in the command line spaced our or in a file line by line (ex: `examples/centers.txt`), each point in form of `x_1,x_2,...,x_i`

- `--box [-b] {required}`: bounding box of the Voronoi diagram in the form of two points (ex: `0,0 10,10`, `3,4,6 10,45,100`)

- `--iterations [-i] {optional}`: number of iterations of relaxation to do, if used with `convergence` - 

- `--convergence [-v] {optional, default: 1}`: when the distance between two center is less than this, terminates

- `--output [-o] {optional}`: path to save outputs, by default prints to stdout

- `--full [-f] {optional}`: to save each iteration of relaxation, or just the final iteration

- `--override [-r] {optional}`: if file under `--output` already exists, override it

The results for each iteration are save in the following format:
- each iteration is separated by a blank line
- within each iteration, clusters / cells are on their own line
- clusters / cells are in the format of "center point | [points in cluster]"

for if *i* is the number of dimensions and *j* is the number of center points, the results for a single iteration may look like
```
c1_1,c1_2,...,c1_i | a1_1,a1_2,...,a2_i b2_1,b2_2,...,b1_i ...
c1_1,c1_2,...,c1_i | a2_1,a2_2,...,a2_i b2_1,b2_2,...,b2_i ...
...
cj_1,cj_2,...,cj_i | aj_1,aj_2,...,aj_i bj_1,bj_2,...,bj_i ...
```

A small Python script under `display_results.py` is included to display the results that are in 2D. It takes in a results file and directory to save the resulting images. Requires Python 3.9 and Pillow.


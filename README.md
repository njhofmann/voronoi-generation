# voronoi-generation
Point-by-point, *n*-dimensional Voronoi diagram generation and relaxation in C.

<p align="center">
  <img width="400" src="https://media3.giphy.com/media/9FaM4l3F6FLsjvatmk/giphy.gif?cid=790b761167f73c5c8d9807451e5d20ddfc6e121b299f8d03&rid=giphy.gif&ct=g" alt="header">
</p>

## Background 

This is small C program for [Voronoi diagram](https://en.wikipedia.org/wiki/Voronoi_diagram) generation and [relaxation](https://en.wikipedia.org/wiki/Lloyd%27s_algorithm) under a variety of distance functions, generalized to *n*-dimensions.

Instead of utilizing algorithms like [Fortune's algorithm](https://en.wikipedia.org/wiki/Fortune%27s_algorithm) that use geometric structures, this implementation opts for a coordinate based implementation - i.e. using points like (1, 1) and (2, 2) directly. The resulting algorithm is slower (~*n*^*k* where *n* is the size of each dimension and *k* is the number of dimensions) and more of an approximation, but is vastly simpler in its implementation, output, and ability to generalize to higher dimensions.

## Usage

This project uses CMake to build the executable`voronoi_generation`.

The executable accepts the following arguments in any order:

- `--distance [-d] {optional, default: euclidian}`: distance metric to use, see available options under the `Distance Metrics` section

- `--centers [-c] {required}`: starting center points each in form of `x_1,x_2,...,x_i`, entered as:
  - in the command line spaced out
  - in a file line by line (ex: `examples/centers.txt`)
  - a single digit to generate random starting centers
    - dimensions are inferred from `--box`

- `--box [-b] {required}`: bounding box of the Voronoi diagram in the form of an upper right point (ex: `10,10`, `10,45,100`)

- `--iterations [-i] {optional}`: number of iterations of relaxation to do, if terminate earlier if used with the `convergence` argument

- `--convergence [-v] {optional, default: 1}`: terminates when the distance between any two centers is less than this

- `--output [-o] {optional}`: directory to save outputs, by default prints to stdout

- `--p [-p] {optional}`: additional parameter for Mirkowski and Yang distances, defaults to 2

- `--full [-f] {optional}`: flag that if given saves each iteration of relaxation, else saves just the final iteration

- `--override [-r] {optional}`: if file under `--output` already exists, override it

- `--processes [-m] {optional, default: 1}`: number of processes to create for multiprocessing, doesn't add much benefit

### Distance Metrics

Program supports the following distance metrics, enter them into `--distance` as presented:

- [euclidean](https://en.wikipedia.org/wiki/Euclidean_distance) - Minkowski distance when *p*=2
- [manhattan](https://en.wikipedia.org/wiki/Taxicab_geometry) - Minkowski distance when *p*=1
- ramanujan
- [chebyshev](https://en.wikipedia.org/wiki/Chebyshev_distance) - Minkowski distance when *p*=infinite
- [canberra](https://en.wikipedia.org/wiki/Canberra_distance) - weighted version of Manhattan distance
- [pearson](https://en.wikipedia.org/wiki/Pearson_correlation_coefficient#Pearson's_distance) - Pearson's correlation coefficient as a metric
- [cosine](https://en.wikipedia.org/wiki/Cosine_similarity) - not a proper distance metric because it doesn't obey the triangle inequality
- [bray-curtis](https://docs.scipy.org/doc/scipy/reference/generated/scipy.spatial.distance.braycurtis.html#scipy.spatial.distance.braycurtis) - not a proper distance metric because it doesn't obey the triangle inequality 
- [minkowski](https://en.wikipedia.org/wiki/Minkowski_distance)
- [yang](https://www.ccs.neu.edu/home/radivojac/papers/yang_dataminknowldisc_2019.pdf)

### Output

`voronoi_generation` produces two outputs:

#### Cell Centers

The center points for every cell are written as

```
center_1_1 center_1_2 ... center_1_n
center_2_1 center_2_2 ... center_2_n
...
center_m_1 center_m_2 ... center_m_n
```

where `center_i_j` is the center point of the *j*-th cluster in the *i*-th iteration.

#### Point Groups

The cells each point is a part of is written as - if there are *m* iterations and *n* points:

```
point_1 point_1_cell_1 point_1_cell_2 ... point_1_cell_m
point_2 point_2_cell_1 point_2_cell_2 ... point_2_cell_m
...
point_n point_n_cell_1 point_n_cell_2 ... point_n_cell_m
```

where `point_i_cell_j` is the cell the *i*-th point was a part of during the *j*-th iteration as represented by an integer *k*. The exact center can be found in the *j*-th row and *k*-th column of the cell centers output.

### Visualization

A small Python script `display_results.py` is included to visualize 2D and 3D results. It takes in the output files from a run of `voronoi_generation` among other arguments, a directory to save the resulting images, and other flag. Use `python display_results.py -h` for a full list of arguments.

Requires Python 3.9, Pillow 8.3.2, and Matplotlib 3.5.0.
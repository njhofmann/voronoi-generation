1. arg parser
    1. list out all needed args to describe expected input & output
    1. args
        1. distance metric
            1. euclidean, manhattan, mahalanobis, ramanujan's oval perimeter estimate
        1. staring centers
            1. list of or path to file of n-d points
                1. path formatted as 1 line = 1 center
            1. positive int, number of center points to randomly generate
        1. bounding box - boundary of diagram
            1. two n-d points, one the "upper left" one the "lower right"
                1. what does a validity check look like?
        1. iterations - voronoi diagrams to iteratively generate
            1. default = 0, create diagram using only starting centers
            1. -1, iterate until convergence
            1. some other positive int
        1. convergence - threshold to stop convergence
            1. positive float, default .001
            1. if non-positive, use only iterations
        1. output file path
            1. if not given, spit output to terminal
        1. show all iterations
            1. output only final diagram, or each intermediate diagram

1. to do
    1. 3d display
    2. random centers
    3. random colors
    4. k-nearest point Voronoi diagram
    5. k-th order Voronoi diagram, points in cells share the nearest k centers
    6. plot ideas:
       1. run time vs num of points
       2. runtime vs processes
       3. num of centers runtime vs num of processes
    7. valgrind warnings
    8. help option
    9. docs
    10. read me
    11. when does it fail?
    12. redo matrix width
         1. make rows point to null, only append
         1. tension between int arrays that are being resized and int matrix
         1. how to create a new empty int matrix with a "width" if width relies on int arrays

1. misc
    1. higher order voronoi diagrams
    1. weighted diagrams
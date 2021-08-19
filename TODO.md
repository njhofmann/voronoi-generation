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
    4. valgrind warnings
    6. where does multiprocessing actually help? do some time trials
       1. i = 1, 5, 10, 20
       2. m = 1, 2, 4, 8?
       3. \# of points = 100,100 to 1000,1000 (log)
       4. \# of centers = 2, 5, 10, 15, 20, 25
    7. help option
    8. docs
    9. read me
    10. random centers
    12. when does it fail?
    13. valid centers chec
    14. redo matrix width
         1. make rows point to null, only append
         1. tension between int arrays that are being resized and int matrix
         1. how to create a new empty int matrix with a "width" if width relies on int arrays

1. misc
    1. higher order voronoi diagrams
    1. weighted diagrams
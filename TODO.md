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

1. output
    1. each iteration is a 'mapping' from center if points in that cell

1. to do
    1. multithreading
    1. boolean options for arg parser

1. algorithms
    1. naive generation
    1. lloyd's / Linde–Buzo–Gray algorithm
    1. fortune's
    1. Bowyer–Watson algorithm

1. misc
    1. higher order voronoi diagrams
    1. weighted diagrams
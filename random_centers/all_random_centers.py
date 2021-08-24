import sys
import random_centers as rc

if __name__ == '__main__':
    output_dirc = sys.argv[1]
    dims = int(sys.argv[2])
    for n in range(1, 7):
        n = 2 ** n
        for bound in range(100, 1501, 100):
            bounds = tuple([bound for _ in range(dims)])
            rc.write_centers(n, bounds, output_dirc)

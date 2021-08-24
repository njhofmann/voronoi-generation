import random as r
import sys


def generate_random_centers(num: int, bounds: tuple[int, ...]) -> list[tuple[int, ...]]:
    return [tuple([r.randint(0, bound - 1) for bound in bounds]) for _ in range(num)]


def save_random_centers(centers: list[tuple[int, ...]], output_path: str) -> None:
    with open(output_path, 'w') as f:
        for center in centers:
            f.write(','.join([str(x) for x in center]) + '\n')


def parse_bounds(raw_bounds: str) -> tuple[int, ...]:
    return tuple([int(x) for x in raw_bounds.split(',')])


def write_centers(num: int, bounds: tuple[int, ...], output_dirc: str):
    centers = generate_random_centers(num, bounds)
    output_path = f'{output_dirc}/{num}_in_{"-".join([str(x) for x in bounds])}.txt'
    save_random_centers(centers, output_path)


if __name__ == '__main__':
    num_of_centers = int(sys.argv[1])
    bounds = parse_bounds(sys.argv[2])
    output_dirc = sys.argv[3]
    write_centers(num_of_centers, bounds, output_dirc)

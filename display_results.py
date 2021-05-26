import PIL.Image as pi
import pathlib as pl
import sys

IMG_DIRC = pl.Path('imgs')


def create_point(raw_pt: str) -> list[int]:
    return list(map(int, filter(lambda x: bool(x) and x != '\n', raw_pt.split(','))))


def load_centers(path: str) -> list[list[list[int]]]:
    all_centers = []
    with open(path, 'r') as f:
        for line in f.readlines():
            centers = list(map(create_point, line.split(' ')))
            centers.pop()
            all_centers.append(centers)
    return all_centers


def load_colors(path: str) -> list[list[int]]:
    with open(path, 'r') as f:
        return [create_point(x) for x in f.readlines()]


def load_point_groups(path: str) -> list[tuple[list[int], list[int]]]:
    points_and_centers = []
    with open(path, 'r') as f:
        for line in f.readlines():
            temp = line.split(' ')
            point = create_point(temp[0])
            centers = list(map(int, temp[1:-1]))
            points_and_centers.append((point, centers))
    return points_and_centers


def display_voronoi(point_groups: list[tuple[list[int], list[int]]], centers: list[list[list[int]]],
                    colors: list[list[int]], save_path: str) -> None:
    size = tuple([x + 1 for x in point_groups[-1][0]])
    imgs = [pi.new('RGB', size) for _ in range(len(point_groups[0][1]))]
    # TODO multiprocessing here
    for point, centers in point_groups:
        point = tuple(point)
        for i, center in enumerate(centers):
            imgs[i].putpixel(point, tuple(colors[center]))
    imgs[0].save(save_path, save_all=True, loop=0, append_images=imgs[1:], )


if __name__ == '__main__':
    point_groups = load_point_groups(sys.argv[1])
    all_centers = load_centers(sys.argv[2])
    colors = load_colors(sys.argv[3])
    save_path = sys.argv[4]
    display_voronoi(point_groups, all_centers, colors, save_path)

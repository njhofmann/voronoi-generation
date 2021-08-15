import PIL.Image as pi
import pathlib as pl
import sys
import multiprocessing as mp
import numpy as np

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


def load_colors(path: str) -> list[tuple[int]]:
    with open(path, 'r') as f:
        return [tuple(create_point(x)) for x in f.readlines()]


def load_point_groups(path: str) -> tuple[list[list[int]], np.ndarray]:
    points = []
    centers = []
    with open(path, 'r') as f:
        for line in f.readlines():
            temp = line.split(' ')
            points.append(create_point(temp[0]))
            centers.append(list(map(int, filter(lambda x: x != '\n', temp[1:]))))
    return points, np.array(centers)


def create_img(img_centers: np.ndarray, points, size) -> pi.Image:
    img = pi.new('RGB', size)
    for j, point in enumerate(points):
        img.putpixel(point, colors[img_centers[j]])
    return img


def display_voronoi(points: list[list[int]], point_centers: np.ndarray, real_centers: list[list[list[int]]],
                    colors: list[list[int]], save_path: str, process_cnt: int) -> None:
    # TODo multiprocessing here
    size = tuple([x + 1 for x in points[-1]])
    if process_cnt == 1:
        imgs = []
        # TODO multiprocessing here
        for i in range(len(point_centers[0])):
            img = pi.new('RGB', size)
            img_centers = point_centers[:, i]
            for j, point in enumerate(points):
                img.putpixel(point, colors[img_centers[j]])
            imgs.append(img)

    elif process_cnt > 1:
        with mp.Pool(processes=process_cnt) as p:
            imgs = p.starmap(func=create_img, iterable=[(x, points, size) for x in point_centers.T])
    else:
        raise ValueError('need positive number for # of processes')
    # for point, centers in point_groups:
    #     point = tuple(point)
    #     for i, center in enumerate(centers):
    #         imgs[i].putpixel(point, tuple(colors[center]))
    imgs[0].save(save_path, save_all=True, loop=0, append_images=imgs[1:], )


if __name__ == '__main__':
    points, point_centers = load_point_groups(sys.argv[1])
    all_centers = load_centers(sys.argv[2])
    colors = load_colors(sys.argv[3])
    save_path = sys.argv[4]
    process_cnt = 4 # TODO make this arg
    display_voronoi(points, point_centers, all_centers, colors, save_path, process_cnt)

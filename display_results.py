import argparse as ap
import enum as e
import multiprocessing as mp
import math as m
import pathlib as pl
import random as r
from typing import Union

import matplotlib.pyplot as plt
import numpy as np
import PIL.Image as pi
import PIL.ImageDraw as id

IMG_DIRC = pl.Path('imgs')

class ViewMode(e.Enum):
    CELLS = 'cells'  # display only cells
    CENTERS = 'centers'  # display changes in centers across iterations
    CELLS_WITH_CENTERS = 'cells_with_centers'  # display cells with their centers

    @classmethod
    def str_to_mode(cls, string):
        for mode in cls:
            if mode.value == string:
                return mode
        raise ValueError(f'string {string} is an invalid {str(cls)}')


def parse_point(raw_pt: str) -> tuple[int, ...]:
    return tuple(map(int, filter(lambda x: bool(x) and x != '\n', raw_pt.split(','))))


def load_centers(path: str) -> list[list[tuple[int, ...]]]:
    all_centers = []
    with open(path, 'r') as f:
        for line in f.readlines():
            centers = list(map(parse_point, line.split(' ')))
            centers.pop()
            all_centers.append(centers)
    return all_centers


def load_colors(path: Union[str, int]) -> list[tuple[int, int, int]]:
    if isinstance(path, int):
        return [tuple([r.randint(0, 255) for _ in range(3)]) for _ in range(path)]
    with open(path, 'r') as f:
        return [tuple(parse_point(x)) for x in f.readlines()]


def load_point_groups(path: str) -> tuple[list[tuple[int, ...]], np.ndarray]:
    points, point_groups = [], []
    with open(path, 'r') as f:
        for line in f.readlines():
            temp = line.split(' ')
            points.append(parse_point(temp[0]))
            point_groups.append(list(map(int, filter(lambda x: x != '\n', temp[1:]))))
    return points, np.array(point_groups)


def create_img(img_centers: np.ndarray, points, colors, size) -> pi.Image:
    img = pi.new('RGB', size)
    for j, point in enumerate(points):
        img.putpixel(point, colors[img_centers[j]])
    return img


def create_2d_voronoi_diagram(points: list[tuple[int, ...]],
                              point_groups: np.ndarray,
                              group_centers: list[list[tuple[int, ...]]],
                              colors: list[tuple[int, int, int]],
                              process_cnt: int,
                              size: tuple[int, int]) -> list[pi.Image]:
    if process_cnt == 1:
        return [create_img(point_groups[:, i], points, colors, size) for i in range(len(point_groups[0]))]
    elif process_cnt > 1:
        with mp.Pool(processes=process_cnt) as p:
            return p.starmap(func=create_img, iterable=[(x, points, colors, size) for x in point_groups.T])
    raise ValueError('need positive number for # of processes')


def create_center_lines(centers, size) -> pi.Image:
    img = pi.new('RGB', size, (255, 255, 255))
    draw = id.Draw(img)
    line_width = max([m.ceil(.01 * x) for x in size])
    for group in centers:
        # points = [tuple(x) for x in group]
        #draw.point(xy=points, fill='red')  # TODO here
        draw.line(list(map(tuple, group)), fill='black', width=line_width)
    return img

def create_2d_centers_over_time(centers: list[list[tuple[int, ...]]], size: tuple[int, int]) -> list[pi.Image]:
    centers = np.array(centers)
    n_iters, n_groups = centers.shape[:2]
    centers = np.array([centers[:, i, :] for i in range(n_groups)])
    return [create_center_lines(centers[:, :i, :], size) for i in range(1, n_iters)]

def create_2d(points: list[tuple[int, ...]],
              point_groups: np.ndarray,
              group_centers: list[list[tuple[int, ...]]],
              colors: list[tuple[int, int, int]],
              mode: ViewMode,
              save_path: str,
              process_cnt: int) -> None:
    size = tuple([x + 1 for x in points[-1]])
    if mode == ViewMode.CELLS:
        imgs = create_2d_voronoi_diagram(points, point_groups, group_centers, colors, process_cnt, size)
    elif mode == ViewMode.CELLS_WITH_CENTERS:
        pass
    else:  # CENTERS
        imgs = create_2d_centers_over_time(group_centers, size)

    imgs[0].save(save_path, save_all=True, loop=0, append_images=imgs, format='GIF')


def create_3d_centers_over_time(centers: list[list[tuple[int, ...]]], axes: plt.Axes) -> None:
    centers = np.array(centers)
    for i in range(len(centers[0])):
        line = centers[:, i, :]
        axes.plot(line[:, 0], line[:, 1], line[:, 2])


def create_3d_voronoi_diagram(points: list[tuple[int, ...]],
                              point_groups: np.ndarray,
                              colors: list[tuple[int, int, int]],
                              save_path: str,
                              axes: plt.Axes):
    # only creates a graph for the last image
    last_point = points[-1]

    face_colors = np.zeros((*[x + 1 for x in last_point], 3), dtype=np.float32)
    for i, point in enumerate(points):
        face_colors[point] = colors[point_groups[:, -1][i]] #colors[]

    filled = np.full(shape=[x + 1 for x in last_point], fill_value=True)  # every voxel is full

    axes.voxels(filled=filled, facecolors=face_colors / 255)


def create_3d(points: list[tuple[int, ...]],
              point_groups: np.ndarray,
              group_centers: list[list[tuple[int, ...]]],
              colors: list[tuple[int, int, int]],
              mode: ViewMode,
              save_path: str,
              process_cnt: int) -> None:
    fig = plt.figure()
    axes = fig.add_subplot(projection='3d')

    if mode == ViewMode.CELLS:
        create_3d_voronoi_diagram(points, point_groups, colors, save_path, axes)
    elif mode == ViewMode.CENTERS:
        create_3d_centers_over_time(group_centers, axes)
    else:
        raise ValueError(f'view mode {ViewMode.CELLS_WITH_CENTERS} is not supported for 3D graphs')

    axes.grid(visible=True)
    axes.axis('tight')
    fig.savefig(save_path)
    plt.show()

def create_voronoi_diagram(points: list[tuple[int, ...]],
                           point_groups: np.ndarray,
                           group_centers: list[list[tuple[int, ...]]],
                           colors: list[tuple[int, int, int]],
                           mode: ViewMode,
                           save_path: str,
                           process_cnt: int) -> None:
    point_dims = len(points[0])
    if point_dims == 2:
        display_func = create_2d
    elif point_dims == 3:
        display_func = create_3d
    else:
        raise ValueError('only support the displaying of 2D & 3D points')

    display_func(points, point_groups, group_centers, colors, mode, save_path, process_cnt)


def get_user_args() -> ap.Namespace:
    parser = ap.ArgumentParser()
    parser.add_argument('--point_centers', required=True, help='path to output file of points & their assigned cells')
    parser.add_argument('--centers', required=True,
                        help='path to output file out centers for each cell at each iteration')
    parser.add_argument('--colors', default=None, help='optional path to file of colors')
    parser.add_argument('--save_path', default='img.gif', help='path to save resulting GIF')
    parser.add_argument('--processes', default=4, type=int)
    parser.add_argument('--mode', default=ViewMode.CELLS, type=ViewMode.str_to_mode, help='what mode')
    return parser.parse_args()


if __name__ == '__main__':
    user_args = get_user_args()
    points, cell_idxs = load_point_groups(user_args.point_centers)
    cell_centers = load_centers(user_args.centers)

    colors = user_args.colors
    if colors is None:
        colors = len(cell_idxs[0])

    colors = load_colors(colors)
    create_voronoi_diagram(points=points,
                           point_groups=cell_idxs,
                           group_centers=cell_centers,
                           colors=colors,
                           mode=user_args.mode,
                           save_path=user_args.save_path,
                           process_cnt=user_args.processes)

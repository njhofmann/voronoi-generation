import dataclasses as dc
import math as m
from typing import Optional

import PIL.Image as pi
import random as r
import pathlib as pl
import sys

IMG_DIRC = pl.Path('imgs')


@dc.dataclass(frozen=True)
class Cluster:
    """A cluster of cells, assumes each cell has the same # of dimensions"""
    center: list[int]
    cells: list[list[int]]


@dc.dataclass()
class ClusterLineage:
    color: tuple[int, int, int] = None
    cluster_lineages: list[Cluster] = None

    def __post_init__(self):
        self.clusters = []

    def add_cluster(self, cluster: Cluster) -> None:
        self.clusters.append(cluster)
        self.color = tuple([r.randint(0, 255) for _ in range(3)])


def create_point(raw_pt: str) -> list[int]:
    return list(map(int, filter(lambda x: bool(x) and x != '\n', raw_pt.split(','))))


def create_cluster(line: str) -> Cluster:
    points = line.split(' ')
    center = create_point(points[0])
    cells = list(filter(bool, map(create_point, points[2:])))
    return Cluster(center, cells)


def cluster_dist(a: Cluster, b: Cluster) -> float:
    return sum([(a.center[i] - b.center[i]) ** 2 for i in range(len(a.center))]) ** 1 / 2


def find_closest_cluster(cluster: Cluster, clusters: list[Cluster]) -> tuple[Cluster, list[Cluster]]:
    min_dist, min_idx = m.inf, None
    for i, other_cluster in enumerate(clusters):
        if (dist := cluster_dist(cluster, other_cluster)) < min_dist:
            min_dist = dist
            min_idx = i
    closest = clusters.pop(min_idx)
    return closest, clusters


def clusters_to_lineages(clusters: list[list[Cluster]]) -> list[ClusterLineage]:
    lineages = []
    while len(clusters[0]) > 0:
        lineage = ClusterLineage()
        cur_gen_cluster = clusters[0].pop()
        lineage.add_cluster(cur_gen_cluster)

        for i in range(1, len(clusters)):
            cur_gen_cluster, clusters[i] = find_closest_cluster(cur_gen_cluster, clusters[i])
            lineage.add_cluster(cur_gen_cluster)

        lineages.append(lineage)

    assert all([len(x) == 0 for x in clusters])
    return lineages


def create_cluster_lineages(clusters_file: str) -> list[ClusterLineage]:
    lineages = []
    with open(clusters_file, 'r') as f:
        cur_gen = []
        for line in f.readlines():
            if line == '\n':
                lineages.append(cur_gen)
                cur_gen = []
            else:
                cur_gen.append(create_cluster(line))

    return clusters_to_lineages(lineages)


def display_gen(gen: list[tuple[Cluster, str]], save_path: pl.Path, size: Optional[tuple[int, int]] = None) \
        -> tuple[int, int]:
    if size is None:
        size = tuple(map(lambda x: max(x) + 1, zip(*gen[0][0].cells)))
    img = pi.new('RGB', size)
    for cluster, color in gen:
        for cell in cluster.cells:
            img.putpixel(tuple(cell), color)
    img.save(save_path)
    return size


def display_lineages(lineages: list[ClusterLineage], save_dirc: str) -> None:
    save_dirc = IMG_DIRC / save_dirc
    save_dirc.mkdir(parents=True, exist_ok=True)
    size = None
    for i in range(len(lineages[0].clusters)):
        cur_gen = [(lineages[j].clusters[i], lineages[j].color) for j in range(len(lineages))]
        size = display_gen(cur_gen, save_dirc / f'{i}.png', size)


if __name__ == '__main__':
    save_dirc_name = 'test'
    cluster_lineages = create_cluster_lineages(sys.argv[1])
    display_lineages(cluster_lineages, save_dirc_name)

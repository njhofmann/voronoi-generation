import random as r
import argparse as ap
import pathlib as pl

COLORS = [('red', (255, 0, 0)), ('blue', (0, 0, 255)), ('green', (0, 255, 0)), ('yellow', (255, 255, 0)),
          ('orange', (255, 165, 0)), ('black', (0, 0, 0)), ('white', (255, 255, 255)), ('purple', (128, 0, 128))]


def generate_colors(central_color: tuple[int, int, int], x: int, n: int, save_path: pl.Path) -> None:
    with open(save_path, 'w') as f:
        for _ in range(x):
            new_color = tuple([max(0, min(255, x + r.randrange(-1 * n, n + 1))) for x in central_color])
            f.write(f'{new_color[0]},{new_color[1]},{new_color[2]}\n')


def parse_color(color: str) -> tuple[tuple[int, int, int], pl.Path]:
    save_dirc = pl.Path(f'colors')
    save_dirc.mkdir(exist_ok=True, parents=True)

    color = color.split(' ')
    if len(color) == 1:
        color = color[0]
        for c, rgb in COLORS:
            if c == color:
                return rgb, save_dirc.joinpath(f'{color}.txt')
    elif len(color) == 3:
        color = tuple(map(int, color))
        return color, save_dirc.joinpath('color.txt')


def get_args() -> ap.Namespace:
    parser = ap.ArgumentParser(
        usage='generate X colors whose RGB coordinates are within N of the coordinates of given color')
    parser.add_argument('--color', type=parse_color, help='color to center around')
    parser.add_argument('--x', type=int, default=50, help='# of colors to generate')
    parser.add_argument('--n', type=int, default=30, help='bounds')
    return parser.parse_args()


if __name__ == '__main__':
    args = get_args()
    assert args.n > 0, 'bounds must be positive'
    central_color, save_path = args.color
    generate_colors(central_color, args.x, args.n, save_path)

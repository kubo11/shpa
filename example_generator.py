from argparse import ArgumentParser, Namespace
from pathlib import Path
from random import randint

class RandomIncidenceArray:
    def __init__(self, n: int, min_weight: int, max_weight: int) -> None:
        self._incidence_array = \
            [[randint(min_weight, max_weight) for col in range(row+1)] for row in range(n-1)]

    def __str__(self) -> str:
        out = str(len(self._incidence_array)+1)
        for row in self._incidence_array:
            out += '\n'
            for w in row:
                out += f'{w} '
        return out

def print_usage() -> None:
    print('usage:')
    print('example_generator.py size [path]')
    print('\tsize - number of vertices for which graph should be generated')
    print('\tpath (optional) - path to output file (default: test_<size>.txt)')

def parse_args() -> Namespace:
    parser = ArgumentParser()
    parser.add_argument('-v', '--vertices', type=int, required=True, help='number of vertices for which graph should be generated')
    parser.add_argument('-o', '--output', type=Path, required=False, default=None, help='path to output file (default: test_<size>.txt)')
    parser.add_argument('-m', '--min-weight', type=int, required=False, default=1, help='minimal weight')
    parser.add_argument('-x', '--max-weight', type=int, required=False, default=100, help="maximal weight")

    return parser.parse_args()

def save(r: RandomIncidenceArray, path: str) -> None:
    with open(path, "w") as file:
        file.write(str(r))

def main() -> None:
    args = parse_args()
    if args.output is None:
        args.output = f'test{args.vertices}.txt'

    save(RandomIncidenceArray(args.vertices, args.min_weight, args.max_weight), args.output)

if (__name__ == '__main__'):
    main()
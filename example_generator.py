from sys import argv
from random import randint

MIN_WEIGHT = 1
MAX_WEIGHT = 10_000

class RandomIncidenceArray:
    def __init__(self, n: int) -> None:
        self._incidence_array = \
            [[randint(MIN_WEIGHT, MAX_WEIGHT) for col in range(row+1)] for row in range(n-1)]

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

def save(r: RandomIncidenceArray, path: str) -> None:
    with open(path, "w") as file:
        file.write(str(r))

def main() -> None:
    if len(argv) == 2:
        save(RandomIncidenceArray(int(argv[1])), f'test{argv[1]}.txt')
    elif len(argv) == 3:
        save(RandomIncidenceArray(int(argv[1])), argv[2])
    else:
        print('Invalid number of arguments')
        print_usage()

if (__name__ == '__main__'):
    main()
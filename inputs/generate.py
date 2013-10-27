#!/usr/bin/env python
"""Generate random matrices for multiplication.

Usage:
    ./generate.py <M> <K> <N>

The first line of output will contain simply:

    M K N

Next will be an M by K matrix, followed by a K by N matrix.

Values are floats between 0 and 1.

"""
from __future__ import print_function
from sys import argv, stderr
from random import random

def print_matrix(m, n):
    for i in range(m):
        for j in range(n):
            print(random(), "", end="")
        print()

if __name__ == '__main__':
    if len(argv) != 4:
        print("Usage: {} <M> <K> <N>".format(argv[0]),
                file=stderr)

    M, K, N = map(int, argv[1:4])
    print(M, K, N)
    print_matrix(M, K)
    print_matrix(K, N)

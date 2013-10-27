from __future__ import print_function
from sys import argv, stderr, exit

def read_toks(fp):
    return fp.readline().split()
def read_ints(fp):
    return map(int, read_toks(fp))
def read_floats(fp):
    return map(float, read_toks(fp))
def read_matrix(fp, rows):
    return [read_floats(fp) for i in range(rows)]

if len(argv) != 2:
    print("Usage: {} <input>".format(argv[0]),
            file=stderr)
    exit(1)

filename = argv[1]
with open(filename) as fp:
    m, k, n = read_ints(fp)
    a = read_matrix(fp, m)
    b = read_matrix(fp, k)

c = [[0 for i in range(n)] for i in range(m)]
for i in range(m):
    for j in range(k):
        for h in range(n):
            c[i][j] += a[i][h] * b[h][j]

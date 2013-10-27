from json import loads, dumps
from numpy import array, dot
from sys import stdin, stdout

args = loads(stdin.read())
a, b = map(array, (args['a'], args['b']))
c = dot(a, b)
result = dumps({'c': c.tolist()})
stdout.write(result)
stdout.flush()

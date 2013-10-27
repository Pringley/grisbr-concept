# Grisbr: proof of concept

This is a proof-of-concept bridge between Ruby and Python to perform matrix
multiplication.

**Motivation:** The power of a scripting language lies partially in its
community-developed libraries. Different languages have different strengths --
for example, Python has excellent numeric and scientific packages that Ruby
mostly lacks. Is there a simple way to use packages from one scripting language
for projects written in a different scripting language?

## Introduction

In Ruby, matrix multiplication is done via the
[Matrix#*](http://www.ruby-doc.org/stdlib-2.0.0/libdoc/matrix/rdoc/Matrix.html#method-i-2A)
method:

    require 'matrix'
    a = Matrix[[1, 2], [3, 4]]
    b = Matrix[[4, 3], [2, 1]]
    puts a * b
    # => Matrix[[8, 5], [20, 13]]

This works for small matrices, but since it is implemented in pure Ruby, it can
be very slow. For example, multipling 256x256 matrices:

    $ cd ruby
    $ time ruby mm-native.rb ../inputs/512.txt
    ruby mm-native.rb ../inputs/512.txt  45.45s user 0.04s system 99% cpu 45.502 total

How can we speed this up?

We could write the function in C, which Ruby supports, but this is both tedious
and error-prone. We're using a scripting language for a reason!

Fortunately, Ruby has a fast *and* high-level matrix implementation in the
works -- it's called [NMatrix](http://sciruby.com/nmatrix/) from
[SciRuby](http://sciruby.com/). Unfortunately, this is still alpha software.

On the other hand, Python has [NumPy](http://www.numpy.org/), which is both
stable and feature-rich.

For a quick project, it would be really nice to use NumPy in Ruby.

This proof-of-concept shows that the use of a Ruby-to-Python bridge is a
feasible method for matrix multiplication. The bridge uses
[JSON](http://www.json.org/)-encoded calls over POSIX pipes for communication
between the Ruby process and a forked Python process.

    $ time ruby mm-grisbr.rb ../inputs/512.txt
    ruby mm-grisbr.rb ../inputs/512.txt  1.32s user 0.18s system 101% cpu 1.480 total

## Overview

The Ruby side of the bridge encodes a request as a JSON string with named
arguments. Ruby then forks a Python process with the Python receiver code and
sents the JSON request to the Python process's standard input pipe. After
blocking on computation, Ruby receives the JSON-encoded result back from the
Python process's standard output pipe. This result is then decoded and returned
to the main program.

    require 'open3'

    module Grisbr
      def self.multiply a, b
        args = JSON.generate({a: a, b: b})
        result, status = Open3.capture2("python grisbr-receiver.py",
                                        stdin_data: args)
        JSON.parse(result)["c"]
      end
    end

On the Python side, the JSON-encoded is read on standard input. The arguments
are transformed into NumPy arrays, multiplied, then printed back to standard
output in JSON form.

    from json import loads, dumps
    from numpy import array, dot
    from sys import stdin, stdout

    args = loads(stdin.read())
    a, b = map(array, (args['a'], args['b']))
    c = dot(a, b)
    result = dumps({'c': c.tolist()})
    stdout.write(result)
    stdout.flush()

The bridge is transparent to the library user, who simply uses the `Grisbr`
module:

    a = [[1, 2], [3, 4]]
    b = [[4, 3], [2, 1]]
    p Grisbr.multiply(a, b)
    # => [[8, 5], [20, 13]]

## Analysis

On the 512 by 512 matrix, we saw a 30x speedup using the bridge!

However, on that same matrix, the bridge is still 6 times slower than just
using straight numpy:

    $ cd py
    $ time python mm-numpy.py ../inputs/512.txt 
    python mm-numpy.py ../inputs/512.txt  0.25s user 0.04s system 100% cpu 0.285 total

Also, the time spent marshalling JSON data between processes is wasted. This
means on smaller matricies, native Ruby beats the bridge.

The table below shows a breakdown of runtimes for native Ruby, the bridge, and
straight NumPy on matrices with various sizes:

    +--------+-------+---------+---------+
    |        | 2x2   | 128x128 | 512x512 |
    +========+=======+=========+=========+
    | Native |  .08s |    .79s |  45.50s |
    +--------+-------+---------+---------+
    | Bridge |  .19s |    .27s |   1.48s |
    +--------+-------+---------+---------+
    | Numpy  |  .09s |    .10s |    .28s |
    +--------+-------+---------+---------+

## Alternatives

Specifically for Python-to-Ruby (and vice versa), others have created bridges
using both languages' C integration.

-   [RubyPython](http://rubypython.rubyforge.org/) embeds the Python
    interpreter as a C extension to Ruby.

-   [Unholy](https://github.com/whymirror/unholy) compiles Ruby to Python
    bytecode.

Grisbr instead uses forking and pipes for generality -- this same method could
be used to bridge [Groovy](http://groovy.codehaus.org/) and Python.

## Other Ideas

-   Introspection: should be able to dynamically call any Python/Ruby function

-   Object proxy: when Python returns an object, Ruby should return a proxy

-   Cross-platform: use flat files or database or message bus of some sort
    instead of pipes.

-   Cache method results: save time on obj.len

## Name

Grisbr stands for **Gr**een **Is**land **Br**idge, a vertical lift bridge near
[Rensselaer Polytechnic Institute](http://www.rpi.edu/), the school where the
module was originally developed.

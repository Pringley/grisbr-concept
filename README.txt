# GrIsBr: Green Island Bridge between languages

This is a proof-of-concept bridge between Ruby and Python to perform matrix
multiplication.

## Usage

C:

    cd c
    make
    ./mm <input>

Python:

    cd py
    python mm-native.py <input>
    python mm-numpy.py <input>

Ruby:

    cd rb
    bundle exec ruby mm-native.rb <input>
    bundle exec ruby mm-inline.rb <input>
    bundle exec ruby mm-bridge.rb <input>

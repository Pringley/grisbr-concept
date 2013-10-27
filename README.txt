# GrIsBr: Green Island Bridge between languages

This is a proof-of-concept bridge between Ruby and Python to perform matrix
multiplication.

## Usage

C:

    cd c
    make
    ./mm

Python:

    cd py
    python mm-native.py
    python mm-numpy.py

Ruby:

    cd rb
    bundle exec ruby mm-native.rb
    bundle exec ruby mm-inline.rb
    bundle exec ruby mm-bridge.rb

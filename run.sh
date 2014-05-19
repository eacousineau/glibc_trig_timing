#!/bin/bash

mkdir -p build
cd build
cmake ..
make

# Run the tests
for value in \
    0 1 -1 0.1 0.2 0.3 0.4 0.5 \
    0.93340582292648832662962377071381
do
    ./sintest $value
done

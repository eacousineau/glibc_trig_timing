#!/bin/bash

mkdir -p build
cd build
cmake ..
make

# Run the tests
for value in \
    0 1 -1 \
    0.93340582292648832662962377071381
do
    ./sintest $value
done
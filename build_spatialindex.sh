#!/bin/bash

# build and install spatialindex
pushd lib/spatialindex > /dev/null
mkdir build
pushd build > /dev/null
cmake -D SIDX_BUILD_TESTS=ON ..
make

sudo make install
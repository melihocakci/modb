#!/bin/bash

# builds spatialindex
pushd lib/spatialindex > /dev/null
mkdir -p build
pushd build > /dev/null
cmake -DSIDX_BUILD_TESTS=ON ..
make

# installs spatialindex
sudo make install
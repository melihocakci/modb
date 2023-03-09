#!/bin/bash

# build and install spatialindex
pushd lib/spatialindex > /dev/null
mkdir build
pushd build > /dev/null
cmake ..
make

sudo make install
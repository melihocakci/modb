#!/bin/bash

# changes the starting directory to the script's location
scriptdir=$(dirname $0)
pushd $scriptdir > /dev/null

# get number of processors
processors=$(cat /proc/cpuinfo | grep -c ^processor)

# builds modb
mkdir -p build
pushd build > /dev/null
cmake ..
make -j$processors

#!/bin/bash

sudo apt-get update
sudo apt-get install gnuplot libboost-all-dev libgnuplot-iostream-dev

# changes the starting directory to the script's location
scriptdir=$(dirname $0)
pushd $scriptdir > /dev/null

# get number of processors
processors=$(cat /proc/cpuinfo | grep -c ^processor)

# builds spatialindex
pushd lib/spatialindex > /dev/null
mkdir -p build
pushd build > /dev/null
cmake -D SIDX_BUILD_TESTS=ON ..
make -j$processors

# checks if compilation finished successfully
ret=$?
if [ $ret != 0 ];
then
    exit 255
fi

# installs spatialindex
sudo make install

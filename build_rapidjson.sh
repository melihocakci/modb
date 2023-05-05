#!/bin/bash

sudo apt-get update

# changes the starting directory to the script's location
scriptdir=$(dirname $0)
pushd $scriptdir > /dev/null

# get number of processors
processors=$(cat /proc/cpuinfo | grep -c ^processor)

# builds rapidjson
pushd lib/rapidjson > /dev/null
mkdir -p build
pushd build > /dev/null
cmake ..
make -j$processors

# checks if compilation finished successfully
ret=$?
if [ $ret != 0 ];
then
    exit 255
fi

sudo make install
#!/bin/bash

sudo apt-get update
sudo apt install rapidjson-dev libcurl4-openssl-dev libssl-dev

git submodule update --init --recursive

# changes the starting directory to the script's location
scriptdir=$(dirname $0)
pushd $scriptdir > /dev/null

# get number of processors
processors=$(cat /proc/cpuinfo | grep -c ^processor)

pushd lib/pistache > /dev/null

mkdir -p build
pushd build
cmake ..
make -j$processors

# checks if compilation finished successfully
ret=$?
if [ $ret != 0 ];
then
    exit 255
fi

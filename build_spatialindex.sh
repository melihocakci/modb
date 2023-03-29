#!/bin/bash

if dpkg-query -W -f='${Status}' libgnuplot-iostream-dev | grep -q "install ok installed"; then
  echo "gnuplot is installed"
else
  echo "gnuplot is not installed"
  sudo apt-get update
  sudo apt-get install gnuplot libboost-all-dev libgnuplot-iostream-dev
fi


# changes the starting directory to the script's location
scriptdir=$(dirname $0)
pushd $scriptdir

# builds spatialindex
pushd lib/spatialindex > /dev/null
mkdir -p build
pushd build > /dev/null
cmake -D SIDX_BUILD_TESTS=ON ..
make

# checks if compilation finished successfully
ret=$?
if [ $ret != 0 ];
then
    echo "Compilation failed"
    exit 1
fi

# installs spatialindex
sudo make install
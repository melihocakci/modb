#!/bin/bash

packageInfo=$(dpkg -s libboost-dev)
_=${packageInfo#*:}
isPackagesFound=${packageInfo%"$_"}

if [[ "$isPackagesFound" =~ ^dpkg-query: ]]; then
    echo "Strings are equal"
    sudo apt-get update
    sudo apt-get install gnuplot
    sudo apt-get install libboost-all-dev
    sudo apt-get install libgnuplot-iostream-dev
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
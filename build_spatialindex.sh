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


# builds spatialindex
pushd lib/spatialindex > /dev/null
mkdir -p build
pushd build > /dev/null
cmake -D SIDX_BUILD_TESTS=ON ..
make

# installs spatialindex
sudo make install
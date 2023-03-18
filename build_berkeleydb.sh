#!/bin/bash

# change the starting directory to the script's location
scriptdir=$(dirname $0)
pushd $scriptdir

# builds Berkeley DB
pushd lib/berkeleydb/build_unix > /dev/null
../dist/configure --enable-cxx
make

# checks if compilation finished successfully
ret=$?
if [ $ret != 0 ];
then
    echo "Compilation failed"
    exit 1
fi

# installs Berkeley DB
sudo make prefix=/usr/local install
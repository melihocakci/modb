#!/bin/bash

# builds Berkeley DB
pushd lib/berkeleydb/build_unix > /dev/null
../dist/configure --enable-cxx
make

# checks if compilation finished successfully
ret=$?
if [ $ret != 0 ];
then
    echo "Compilation failed"
    exit
fi

# installs Berkeley DB
sudo make prefix=/usr/local install
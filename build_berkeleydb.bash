#!/bin/bash

# change the starting directory to the script's location
scriptdir=$(dirname $0)
pushd $scriptdir > /dev/null

# get number of processors
processors=$(cat /proc/cpuinfo | grep -c ^processor)

# builds Berkeley DB
pushd lib/berkeleydb/build_unix > /dev/null
mkdir ../lib/berkeleydb/docs/bdb-sql
mkdir ../lib/berkeleydb/docs/gsg_db_server

../dist/configure --enable-cxx
make -j$processors

# checks if compilation finished successfully
ret=$?
if [ $ret != 0 ];
then
    exit 255
fi

# installs Berkeley DB
sudo make prefix=/usr/local install

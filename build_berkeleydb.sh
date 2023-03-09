#!/bin/bash

# build and install berkeleydb
pushd lib/berkeleydb/build_unix > /dev/null
../dist/configure --enable-cxx
make

sudo make prefix=/usr/local \
docdir=/usr/local/docs/berkeleydb \
includedir=/usr/local/include/berkeleydb \
install
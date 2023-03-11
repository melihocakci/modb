#!/bin/bash

# builds berkeleydb
pushd lib/berkeleydb/build_unix > /dev/null
../dist/configure --enable-cxx
make

# installs berkeleydb
sudo make prefix=/usr/local \
docdir=/usr/local/docs/berkeleydb \
includedir=/usr/local/include/berkeleydb \
install
#!/bin/bash

# builds Berkeley DB
pushd lib/berkeleydb/build_unix > /dev/null
../dist/configure --enable-cxx
make

# installs Berkeley DB
sudo make prefix=/usr/local \
docdir=/usr/local/docs/berkeleydb \
includedir=/usr/local/include/berkeleydb \
install
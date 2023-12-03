# MODB (Moving Object Database)

This project implements an abstraction layer for Berkeley DB and libspatialindex that is optimized for moving objects that change position frequently. We do this using a method called QU-trade.

## Building modb

    git submodule update --init --recursive
    ./pre_build.sh
    cmake .
    make

#!/bin/sh

set -e

sudo apt update
sudo apt install -y \
    libdb++-dev \
    libspatialindex-dev \
    libboost-serialization-dev \
    libboost-iostreams-dev \
    libboost-filesystem-dev \

cd $(dirname $0)
git submodule update --init --recursive

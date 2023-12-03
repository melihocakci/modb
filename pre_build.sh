#!/bin/sh

sudo apt update
sudo apt install -y \
    libdb++-dev libspatialindex-dev \
    libboost-serialization-dev libboost-iostreams-dev libboost-filesystem-dev

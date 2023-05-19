#!/bin/bash

scriptdir=$(dirname $0)
pushd $scriptdir

git submodule update --init --recursive

pushd lib/opensky-api/python

sudo apt update
sudo apt install python3 python3-pip
sudo python3 setup.py install

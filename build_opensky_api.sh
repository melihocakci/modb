#!/bin/bash

scriptdir=$(dirname $0)
pushd $scriptdir

pushd lib
git clone https://github.com/openskynetwork/opensky-api

pushd opensky-api/python
sudo apt install python3-pip
sudo python3 setup.py install

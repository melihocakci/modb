# MODB (Moving Object Database)

This project implements an abstraction layer for Berkeley DB and libspatialindex that is optimized for moving objects that change position frequently. We do this using a method called QU-trade.

## Downloading submodules

    git submodule update --init --recursive

## Installing Berkeley DB

    sudo bash -c "./build_berkeleydb.bash"

## Installing libspatialindex

    sudo bash -c "./build_spatialindex.bash"

## Building modb

    ./build_modb.bash

## Other

Executables are stored under build/bin folder

<!-- # why redis in python side 
 * holding data in pc
 * performance comparison of bdb and redis

# Redis wsl2 installation guide
    pip install redis
    sudo apt install redis
    sudo apt isntall redis-server:
    sudo nano /etc/redis/redis.conf //change supervised to systemd 
    sudo service --status-all // check your services for redis -
    sudo /etc/init.d/redis-server start 
    sudo service --status-all // check your services for redis + -->


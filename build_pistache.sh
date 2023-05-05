#!/bin/bash

sudo apt-get update
sudo apt install meson libcurl4-openssl-dev libssl-dev

# changes the starting directory to the script's location
scriptdir=$(dirname $0)
pushd $scriptdir > /dev/null

# get number of processors
processors=$(cat /proc/cpuinfo | grep -c ^processor)

# builds pistache with meson
pushd lib/pistache > /dev/null
meson setup build \
    --buildtype=release \
    -DPISTACHE_USE_SSL=true \
    -DPISTACHE_BUILD_EXAMPLES=true \
    -DPISTACHE_BUILD_TESTS=true \
    -DPISTACHE_BUILD_DOCS=false \
    --prefix="$PWD/prefix"

meson compile -C build
meson install -C build

# checks if compilation finished successfully
ret=$?
if [ $ret != 0 ];
then
    exit 255
fi

export PKG_CONFIG_PATH=~/moving-object-db-system/lib/pistache/prefix/lib/x86_64-linux-gnu/pkgconfig/libpistache.pc:$PKG_CONFIG_PATH

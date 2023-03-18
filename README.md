## Installing Berkeley DB

    sudo bash -c "./build_berkeleydb.sh"

## Installing spatialindex

    sudo bash -c "./build_spatialindex.sh"

## Building modbs

    mkdir -p build
    cd build
    cmake ..
    make
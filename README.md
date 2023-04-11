## Installing Berkeley DB

    sudo bash -c "./build_berkeleydb.sh"

## Installing spatialindex

    sudo bash -c "./build_spatialindex.sh"

## Building modbs

    mkdir -p build
    cd build
    cmake ..
    make

## For using pipe

    cd src/pipe_example
    g++ pipe.cpp -o pipe
    python3 opensky_test.py

# why redis in python side 
 * holding data in pc
 * performance comparison of bdb and redis

# Redis wsl2 installation guide
    pip install redis
    sudo apt install redis
    <p> change supervised to systemd </p>
    sudo nano /etc/redis/redis.conf
    <p> check your services </p>
    sudo /etc/init.d/redis-server start

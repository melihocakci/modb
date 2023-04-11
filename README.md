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
    sudo apt isntall redis-server:
    sudo nano /etc/redis/redis.conf //change supervised to systemd 
    sudo service --status-all // check your services for redis -
    sudo /etc/init.d/redis-server start 
    sudo service --status-all // check your services for redis +


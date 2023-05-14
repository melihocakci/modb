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
 * holding data in main-memory
 * 

# Redis wsl2 installation guide
    pip install redis
    sudo apt install redis
    sudo apt isntall redis-server:
    sudo nano /etc/redis/redis.conf //change supervised to systemd 
    sudo service --status-all // check your services for redis -
    sudo /etc/init.d/redis-server start 
    sudo service --status-all // check your services for redis +

# This repo is overengineering repo
This repo what if situation. somes what if ares
* If we have multiple resources. Now only connected Opensky api for location tracking over planes. We can extend this procedure with other apis such that car tracking opentraffic.io, fleet telemeatics api by HERE tech .
* We have stream data. We can directly publish to frontend map using producer-consumer in kafka(because we have multiple resources with one event bus) and one consumer might publish data. Websocket says that remove that point and add that point to frontend. This is one event that uses producers data. In this way, This system serves real time shows data to web.    
* If person in web site, and want to learn n closest location in particular resource(TODO: we didn't consider different data resource), we serve in rest api. We can see query end points in rest api. 
* 


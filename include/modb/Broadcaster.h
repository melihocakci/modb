// broadcaster broadcast all dataresources messages to kafka producer
// for this purpose, it creates thread for each time data is written to database

// broadcaster has this queue of threads in it and this threads can responsible for serve data to event bus producer.

#ifndef BROADCASTER_H
#define BROADCASTER_H

#include <vector>
#include <cppkafka/cppkafka.h>
#include <future>

namespace modb {

    class Broadcaster // it is singleton 
    {
        public:


        private:
        std::vector<cppkafka::Producer> producer;
    }

}


#endif

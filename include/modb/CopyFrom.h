#ifndef COPYFROM_H
#define COPYFROM_H

#include <stddef.h>
#include <cstdint>
#include <cstring>

namespace modb {
    class CopyFrom {
    public:
        CopyFrom();

        template<typename T> inline void operator()(T object, uint8_t* buffer, size_t len){
            int bufferSize = this->m_offset;
            std::memcpy(object, &buffer[bufferSize], len);
            this->m_offset += len;

        }

        template<typename T> inline void operator()(uint8_t* buffer, T object, size_t len){
            std::memcpy(&buffer[this->m_offset], &object, len);
            this->m_offset += len;

        }

        size_t getOffset();

    private:
        size_t m_offset ;
    };
}

#endif
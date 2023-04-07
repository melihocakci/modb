#ifndef COPYFROM_H
#define COPYFROM_H

#include <stddef.h>
#include <cstdint>

namespace modb {
    class CopyFrom {
    public:
        CopyFrom();

        void operator()(void* object, uint8_t* buffer, size_t len);

        void operator()(uint8_t* buffer, void* object, size_t len);

        size_t getOffset();

    private:
        size_t m_offset;
    };
}

#endif
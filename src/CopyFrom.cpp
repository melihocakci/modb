#include <modb/CopyFrom.h>
#include <cstring>

modb::CopyFrom::CopyFrom(): m_offset{ 0 } {};

void modb::CopyFrom::operator()(void* object, uint8_t* buffer, size_t len) {
    std::memcpy(&object, &buffer[m_offset], len);
    m_offset += len;
}

void modb::CopyFrom::operator()(uint8_t* buffer, void* object, size_t len) {
    std::memcpy(&buffer[m_offset], &object, len);
    m_offset += len;
}

size_t modb::CopyFrom::getOffset() { return m_offset; }
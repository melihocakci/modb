#include <modb/CopyFrom.h>


modb::CopyFrom::CopyFrom(): m_offset(0) {
    int a;
};
size_t modb::CopyFrom::getOffset() { return m_offset; }

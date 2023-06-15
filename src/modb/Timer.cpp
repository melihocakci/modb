#include <modb/Timer.h>

modb::Timer::Timer(int64_t* timer) {
    m_timer = timer;
    m_start = std::chrono::system_clock::now();
}

modb::Timer::~Timer() {
    m_end = std::chrono::system_clock::now();
    auto duration = m_end - m_start;

    (*m_timer) += duration.count();
}

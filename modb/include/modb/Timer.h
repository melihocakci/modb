#ifndef TIMER_H
#define TIMER_H

#include <atomic>
#include <chrono>

namespace modb {
    class Timer {
    public:
        Timer(int64_t* timer);

        ~Timer();

    private:
        int64_t* m_timer;
        std::chrono::_V2::system_clock::time_point m_start;
        std::chrono::_V2::system_clock::time_point m_end;
    };
}

#endif

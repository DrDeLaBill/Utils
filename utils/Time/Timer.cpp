/* Copyright © 2023 Georgy E. All rights reserved. */

#include "Timer.h"

#include <cstdint>

#include "gtime.h"


namespace utl
{
    Timer::Timer(uint32_t delay): delay(delay), start_time(0) { }

    void Timer::start()
    {
        this->start_time = getMillis();
    }

    bool Timer::wait()
    {
        return this->start_time + this->delay < getMillis();
    }
}

/* Copyright © 2023 Georgy E. All rights reserved. */

#include "Timer.h"

#include <cstdint>

#include "Time.h"


namespace utl
{
    Timer::Timer(uint32_t delay): delay(delay), start(0) { }

    void Timer::start()
    {
        this->start = Time::getMillis();
    }

    bool Timer::wait()
    {
        return this->start + this->delay < Time::getMillis();
    }
}

/* Copyright © 2023 Georgy E. All rights reserved. */

#include "Timer.h"

#include <cstdint>

#include "gtime.h"
#include "bmacro.h"


namespace utl
{
    Timer::Timer(uint32_t delay): delay(delay), start_time(0)
	{
    	BEDUG_ASSERT(this->delay > 0, "The delay for the timer must be greater than 0");
	}

    void Timer::start()
    {
        this->start_time = getMillis();
    }

    bool Timer::wait()
    {
        return this->start_time + this->delay < getMillis();
    }
}

/* Copyright © 2023 Georgy E. All rights reserved. */

#include "Timer.h"

#include "gtime.h"
#include "bmacro.h"


namespace utl
{
    Timer::Timer(const TIME_MS_T delay):
        delay(delay), start_time(0)
	{
        BEDUG_ASSERT(this->delay > 0, "The delay for the timer must be greater than 0");
	}

    void Timer::start()
    {
        this->start_time = getMillis();
    }

    void Timer::reset()
    {
    	this->start_time = 0;
    }

    bool Timer::wait()
    {
        return this->start_time + this->delay > getMillis();
    }

    TIME_MS_T Timer::end()
    {
    	return this->start_time + this->delay;
    }

	void Timer::changeDelay(const TIME_MS_T delay_ms)
    {
    	this->delay = delay_ms;
    }
}

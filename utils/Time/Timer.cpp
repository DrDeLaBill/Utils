/* Copyright © 2023 Georgy E. All rights reserved. */

#include "Timer.h"

#include "gtime.h"
#ifdef USE_HAL_DRIVER
#   include "bmacro.h"
#else
#   include "app_exception.h" // TODO
#endif


namespace utl
{
#ifdef USE_HAL_DRIVER
    Timer::Timer(const uint32_t delay):
#else
    Timer::Timer(const unsigned long long delay):
#endif
        delay(delay), start_time(0)
	{
#ifdef USE_HAL_DRIVER
        BEDUG_ASSERT(this->delay > 0, "The delay for the timer must be greater than 0");
#else
        if (this->delay <= 0) {
            throw new exceptions::InternalErrorException();
        }
#endif
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

    void Timer::changeDelay(const unsigned long long delay)
    {
    	this->delay = delay;
    }
}

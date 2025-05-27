/* Copyright © 2023 Georgy E. All rights reserved. */

#include "Timer.h"

#include "gtime.h"


namespace utl
{
    Timer::Timer(const TIME_MS_T delay):
        delay(delay), start_time(0)
	{}

    void Timer::start()
    {
        this->start_time = getMillis();
    }

    void Timer::start(uint32_t delay_ms)
    {
        this->changeDelay(delay_ms);
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
    
    TIME_MS_T Timer::deadline()
    {
        TIME_MS_T end_ms = end();
        if (getMillis() > end_ms) {
            return 0;
        }
        return end_ms - getMillis();
    }
    
    TIME_MS_T Timer::getStart()
    {
        return this->start_time;
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

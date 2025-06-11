/* Copyright © 2023 Georgy E. All rights reserved. */

#include "Timer.h"

#include "gtime.h"


namespace utl
{
    Timer::Timer(const TIME_MS_T delay):
        delay(delay), start_ms(0)
	{}

    void Timer::start()
    {
        this->start_ms = getMillis();
    }

    void Timer::start(uint32_t delay_ms)
    {
        this->changeDelay(delay_ms);
        this->start_ms = getMillis();
    }

    void Timer::reset()
    {
    	this->start_ms = 0;
    }

    bool Timer::wait()
    {
        if (!this->start_ms) {
            return false;
        }
        return this->start_ms + this->delay > getMillis();
    }
    
    TIME_MS_T Timer::deadline()
    {
        if (!this->start_ms) {
            return 0;
        }
        TIME_MS_T end_ms = end();
        if (getMillis() > end_ms) {
            return 0;
        }
        return end_ms - getMillis();
    }
    
    TIME_MS_T Timer::passed()
    {
        if (!this->start_ms) {
            return 0;
        }
        if (getMillis() > this->start_ms) {
            return getMillis() - this->start_ms;
        }
        return 0;
    }
    
    TIME_MS_T Timer::getStart()
    {
        return this->start_ms;
    }

    TIME_MS_T Timer::end()
    {
        if (!this->start_ms) {
            return 0;
        }
    	return this->start_ms + this->delay;
    }

    TIME_MS_T Timer::getDelay()
    {
        return this->delay;
    }

	void Timer::changeDelay(const TIME_MS_T delay_ms)
    {
    	this->delay = delay_ms;
    }
}

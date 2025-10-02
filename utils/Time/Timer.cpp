/* Copyright © 2023 Georgy E. All rights reserved. */

#include "Timer.h"

#include "gtime.h"


namespace utl
{
    Timer::Timer(const uint32_t delay):
        delay(delay), start_ms(0), started(false)
	{}

    void Timer::start()
    {
        this->start_ms = getMillis();
        this->started  = true;
    }

    void Timer::start(uint32_t delay_ms)
    {
        this->changeDelay(delay_ms);
        this->start_ms = getMillis();
        this->started  = true;
    }

    void Timer::reset()
    {
        this->started = false;
    }

    bool Timer::wait()
    {
        if (!this->started) {
            return false;
        }
        return (uint32_t)(this->start_ms + this->delay > getMillis());
    }
    
    uint32_t Timer::deadline()
    {
        if (!this->started) {
            return 0;
        }
        g_time_t end_ms = end();
        if (getMillis() > end_ms) {
            return 0;
        }
        return (uint32_t)(end_ms - getMillis());
    }
    
    uint32_t Timer::passed()
    {
        if (!this->started) {
            return 0;
        }
        if (getMillis() > this->start_ms) {
            return (uint32_t)(getMillis() - this->start_ms);
        }
        return 0;
    }
    
    uint32_t Timer::getStart()
    {
        if (!this->started) {
            return 0;
        }
        return (uint32_t)(this->start_ms);
    }

    uint32_t Timer::end()
    {
        if (!this->started) {
            return 0;
        }
    	return (uint32_t)(this->start_ms + this->delay);
    }
    uint32_t Timer::getDelay()
    {
        return (uint32_t)(this->delay);
    }

	void Timer::changeDelay(const uint32_t delay_ms)
    {
    	this->delay = delay_ms;
    }
}

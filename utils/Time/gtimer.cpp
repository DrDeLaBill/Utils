/* Copyright © 2023 Georgy E. All rights reserved. */

#include "gtimer.h"

#include "gtime.h"


namespace utl
{
    GTimer::GTimer(const uint32_t delay):
        delay(delay), start_ms(0), started(false)
	{}

    void GTimer::start()
    {
        this->start_ms = getMillis();
        this->started  = true;
    }

    void GTimer::start(uint32_t delay_ms)
    {
        this->changeDelay(delay_ms);
        this->start_ms = getMillis();
        this->started  = true;
    }

    void GTimer::reset()
    {
        this->started = false;
    }

    bool GTimer::wait()
    {
        if (!this->started) {
            return false;
        }
        return (uint32_t)(this->start_ms + this->delay > getMillis());
    }
    
    uint32_t GTimer::deadline()
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
    
    uint32_t GTimer::passed()
    {
        if (!this->started) {
            return 0;
        }
        if (getMillis() > this->start_ms) {
            return (uint32_t)(getMillis() - this->start_ms);
        }
        return 0;
    }
    
    uint32_t GTimer::getStart()
    {
        if (!this->started) {
            return 0;
        }
        return (uint32_t)(this->start_ms);
    }

    uint32_t GTimer::end()
    {
        if (!this->started) {
            return 0;
        }
    	return (uint32_t)(this->start_ms + this->delay);
    }
    uint32_t GTimer::getDelay()
    {
        return (uint32_t)(this->delay);
    }

	void GTimer::changeDelay(const uint32_t delay_ms)
    {
    	this->delay = delay_ms;
    }
}

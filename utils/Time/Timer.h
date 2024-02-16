/* Copyright © 2024 Georgy E. All rights reserved. */

#pragma once


#include <cstdint>


namespace utl
{

    class Timer
    {
    private:
#ifdef USE_HAL_DRIVER
        uint32_t delay;
        uint32_t start_time;
#else
        unsigned long long delay;
        unsigned long long start_time;
#endif

    public:
#ifdef USE_HAL_DRIVER
        Timer(uint32_t delay);
#else
        Timer(unsigned long long delay);
#endif
        void start();
        void reset();
        bool wait();

#ifdef USE_HAL_DRIVER
        void changeDelay(const uint32_t delay);
#else
        void changeDelay(const unsigned long long delay);
#endif

    };
}

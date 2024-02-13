/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once


#ifdef USE_HAL_DRIVER
#   include <cstdint>
#endif


namespace utl
{

    class Timer
    {
    private:
#ifdef USE_HAL_DRIVER
        uint32_t delay;
        uint32_t start_time;
#else
        const unsigned long long delay;
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

    };
}

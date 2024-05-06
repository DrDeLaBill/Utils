/* Copyright © 2024 Georgy E. All rights reserved. */

#pragma once


#include <cstdint>


namespace utl
{

    class Timer
    {
    private:
#ifdef USE_HAL_DRIVER
        using time_ms_t = uint32_t;
#else
        using time_ms_t = unsigned long long;
#endif

        time_ms_t delay;
        time_ms_t start_time;

    public:
        Timer(time_ms_t delay);
        
        void start();
        void reset();
        bool wait();
        uint32_t end();

        void changeDelay(const time_ms_t delay_ms);

    };
}

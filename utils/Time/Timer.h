/* Copyright © 2024 Georgy E. All rights reserved. */

#pragma once


#include "gtime.h"


namespace utl
{

    class Timer
    {
    private:
        TIME_MS_T delay;
        TIME_MS_T start_time;

    public:
        Timer(const TIME_MS_T delay);
        
        void start();
        void reset();
        bool wait();
        TIME_MS_T deadline();
        TIME_MS_T end();

        void changeDelay(const TIME_MS_T delay_ms);

    };
}

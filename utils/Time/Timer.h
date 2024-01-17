/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once


#include <cstdint>


namespace utl
{

    class Timer
    {
    private:
        uint32_t delay;
        uint32_t start_time;

    public:
        Timer(uint32_t delay);
        void start();
        void reset();
        bool wait();

    };
}

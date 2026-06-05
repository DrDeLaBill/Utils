/* Copyright © 2024 Georgy E. All rights reserved. */

#pragma once


#include "gtime.h"


namespace utl
{

    class GTimer
    {
    private:
        uint32_t delay;
        uint32_t start_ms;
        bool     started;

    public:
        GTimer(const uint32_t delay = 0);
        
        void start();
        void start(uint32_t delay_ms);
        void reset();
        bool wait();
        uint32_t deadline();
        uint32_t passed();
        uint32_t getStart();
        uint32_t end();
        uint32_t getDelay();

        void changeDelay(const uint32_t delay_ms);

    };
}

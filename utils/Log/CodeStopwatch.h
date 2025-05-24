/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef _CODE_STOPWATCH_T_
#define _CODE_STOPWATCH_T_


#include <cstdint>

#include "gtime.h"


namespace utl
{
    class CodeStopwatch
    {
    public:
        CodeStopwatch(const char* identifier, uint32_t trig_time = 0);
        ~CodeStopwatch();
    private:
        static constexpr unsigned IDENTIFIER_LEN = 6;
        char identifier[IDENTIFIER_LEN];
        TIME_MS_T start;
        TIME_MS_T trig_time;
    };
}


#endif
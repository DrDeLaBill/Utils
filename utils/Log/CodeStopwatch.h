/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once


#include <cstdint>


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
#ifdef USE_HAL_DRIVER
        using time_ms_t = uint32_t;
#else
        using time_ms_t = unsigned long long;
#endif
        time_ms_t start;
        time_ms_t trig_time;
    };
}

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
        uint32_t start;
        uint32_t trig_time;
    };
}

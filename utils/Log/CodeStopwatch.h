/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once


#include <cstdint>
#include <string>

#include "Time.h"


namespace utl
{
    class CodeStopwatch
    {
    public:
        CodeStopwatch(std::string identifier);
        ~CodeStopwatch();
    private:
        std::string identifier;
        const uint32_t start = Time::getMillis();
    };
}
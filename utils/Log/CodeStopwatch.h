/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once


#include <cstdint>
#include <string>


namespace utl
{
    class CodeStopwatch
    {
    public:
        CodeStopwatch(std::string identifier);
        ~CodeStopwatch();
    private:
        std::string identifier;
        uint32_t start;
    };
}
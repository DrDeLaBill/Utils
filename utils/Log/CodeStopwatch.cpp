/* Copyright © 2023 Georgy E. All rights reserved. */

#include "CodeStopwatch.h"

#include <string>
#include <cstdint>

#include "Time.h"
#include "Log.h"


namespace utl
{
    CodeStopwatch::CodeStopwatch(std::string identifier): identifier(std::move(identifier)) { }

    CodeStopwatch::~CodeStopwatch()
    {`
        uint32_t end = Time::getMillis();
        printTagLog<this->identifier.c_str(), end - this->start>("Code execution time for %s identifier: %ld ms");
    }
}
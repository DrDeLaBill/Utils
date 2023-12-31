/* Copyright © 2023 Georgy E. All rights reserved. */

#include "CodeStopwatch.h"

#include <string>
#include <cstdint>

#include "gtime.h"
#include "log.h"


namespace utl
{
    CodeStopwatch::CodeStopwatch(std::string identifier):
        identifier(std::move(identifier)), start(getMillis()) { }

    CodeStopwatch::~CodeStopwatch()
    {
        uint32_t end = getMillis();
        printLog("Code execution time for %s identifier: %ld ms", this->identifier.c_str(), end - this->start);
    }
}
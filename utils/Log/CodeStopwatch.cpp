/* Copyright © 2023 Georgy E. All rights reserved. */

#include <log.h>
#include "CodeStopwatch.h"

#include <memory>
#include <cstring>
#include <cstdint>

#include "utils.h"
#include "gtime.h"


namespace utl
{
    CodeStopwatch::CodeStopwatch(const char* identifier, uint32_t trig_time):
		start(getMillis()), trig_time(trig_time)
    {
    	memset(this->identifier, 0, IDENTIFIER_LEN);
        memcpy(this->identifier, identifier, std::min(strlen(identifier), static_cast<size_t>(IDENTIFIER_LEN)));
	}

    CodeStopwatch::~CodeStopwatch()
    {
    	uint32_t cur_time = getMillis();
        uint32_t delta = __abs_dif(cur_time, start);
        if (delta > this->trig_time) {
            printPretty("Code execution time for %s identifier: %lu ms\n", this->identifier, delta);
        }
    }
}

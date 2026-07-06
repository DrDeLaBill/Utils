/* Copyright © 2023 Georgy E. All rights reserved. */

#include "CodeStopwatch.hpp"

#include <string.h>
#include <stdint.h>

#include "glog.h"
#include "gutils.h"


namespace utl
{
    CodeStopwatch::CodeStopwatch(const char* identifier, uint32_t trig_time):
		start(getMillis()), trig_time(trig_time)
    {
    	memset(this->identifier, 0, IDENTIFIER_LEN);
        memcpy(this->identifier, identifier, __min(strlen(identifier), static_cast<size_t>(IDENTIFIER_LEN) - 1));
	}

    CodeStopwatch::~CodeStopwatch()
    {
#ifdef DEBUG
    	uint32_t cur_time = getMillis();
        uint32_t delta = __abs_dif(cur_time, start);
        if (delta > this->trig_time) {
            printPretty("Code execution time for %s identifier: %lu ms\n", this->identifier, delta);
        }
#endif
    }
}

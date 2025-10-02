/* Copyright © 2023 Georgy E. All rights reserved. */

#include "CodeStopwatch.h"

#include <memory>
#include <cstring>
#include <cstdint>

#include "glog.h"
#include "gutils.h"


namespace utl
{
    CodeStopwatch::CodeStopwatch(const char* identifier, uint32_t trig_time):
		start(getMillis()), trig_time(trig_time)
    {
    	memset(this->identifier, 0, IDENTIFIER_LEN);
        memcpy(this->identifier, identifier, std::min(strlen(identifier), static_cast<size_t>(IDENTIFIER_LEN) - 1));
	}

    CodeStopwatch::~CodeStopwatch()
    {
    	g_time_t cur_time = getMillis();
        g_time_t delta = __abs_dif(cur_time, start);
#ifdef DEBUG
        if (delta > this->trig_time) {
            printPretty("Code execution time for %s identifier: %lu ms\n", this->identifier, delta);
        }
#endif
    }
}

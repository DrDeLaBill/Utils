/* Copyright © 2023 Georgy E. All rights reserved. */

#include <log.h>
#include "bedug.h"



#if defined(_DEBUG) || defined(DEBUG) || !defined(QT_NO_DEBUG)
void b_assert(const char* file, int line, const char* message)
{
    gprint("%s (%d) : bedug assert failed, THERE IS NO REASON TO PANIC, the system is stable : %s\n", file, line, message);
}
#else
void b_assert(const char*, int, const char*)
{
	// TODO: write type of error to the error list and save in memory
}
#endif

/* Copyright © 2023 Georgy E. All rights reserved. */

#include "bedug.h"

#include "log.h"


#if defined(_DEBUG) || defined(DEBUG)
void b_assert(const char* file, int line, const char* message)
{
    print("%s (%d) : bedug assert failed, THERE IS NO REASON TO PANIC, the system is stable : %s\n", file, line, message);
}
#else
void b_assert(const char*, int, const char*)
{
	// TODO: write type of error to the error list and save in memory
}
#endif

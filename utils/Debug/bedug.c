/* Copyright © 2023 Georgy E. All rights reserved. */

#include "bedug.h"

#include "glog.h"


#if defined(_DEBUG) || defined(DEBUG)
void b_assert(const char* file, int line, const char* message)
{
    gprint("%s (%d) : (pu-pu-pu...) %s\n", file, line, message);
}
#else
void b_assert(const char* file, int line, const char* message)
{
	// TODO: write type of error to the error list and save in memory
}
#endif

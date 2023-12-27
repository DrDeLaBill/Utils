/* Copyright © 2023 Georgy E. All rights reserved. */

#include "bedug.h"

#include "log.h"


#if _DEBUG || DEBUG
void b_assert(const char* file, int line, const char* message)
{
    printLog("%s (%ld): bedug assert failed : %s\n", file, line, message);
}
#else
void b_assert(const char*, int, const char*) { }
#endif
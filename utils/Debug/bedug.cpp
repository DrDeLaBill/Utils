/* Copyright © 2023 Georgy E. All rights reserved. */

#include "bedug.h"

#include "Log.h"


namespace bedug
{
#if _DEBUG || DEBUG
    void assert(const char* file, int line, const char* message)
    {
        utl::printLog("%s (%ld): bedug assert failed : %s\n", file, line, message);
    }
#else
    void assert(const char*, int, const char*) { }
#endif
}
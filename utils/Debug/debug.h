/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once


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

#ifndef BEDUG_ASSERT
#   define BEDUG_ASSERT(condition, message) if (!condition) bedug::assert(__FILE__, __LINE__, message);
#endif

#ifndef __STR_DEF2__
#   define __STR_DEF2__(x) #x
#endif

#ifndef __STR_DEF1__
#   define __STR_DEF1__(x) __STR_DEF2__(x)
#endif

#ifndef __LOC__
#   define __LOC__ __FILE__ "("__STR_DEF1__(__LINE__)")"
#endif

#ifndef _WARNING
#   define _WARNING(msgStr) message( __LOC__ " : warning : " msgStr )
#endif
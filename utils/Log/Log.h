/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef __LOG_H
#define __LOG_H


#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>

#include "gtime.h"
#include "bmacro.h"


#ifndef printMessage
#   if defined(__GNUC__)
#       define printMessage(format, ...) printf(format __VA_OPT__(,) __VA_ARGS__);
#   elif defined(_MSC_VER)
#       define printMessage(format, ...) printf(format, ## __VA_ARGS__);
#   else
#       define printMessage(format, ...) { }
#       pragma _WARNING("please check compiler")
#   endif
#endif

#if defined(_DEBUG) || defined(DEBUG)

#   ifndef printTagLog
#       if defined(__GNUC__)
#           define printTagLog(tag, format, ...) printMessage("%08lu->%s:\t" format "\n", getMillis(), tag __VA_OPT__(,) __VA_ARGS__);
#       elif defined(_MSC_VER)
#           define printTagLog(tag, format, ...) printMessage("%08lu->%s:\t" format "\n", getMillis(), tag, ## __VA_ARGS__);
#       endif
#   endif

#   ifndef printLog
#       if defined(__GNUC__)
#           define printLog(format, ...) printMessage(format __VA_OPT__(,) __VA_ARGS__);
#       elif defined(_MSC_VER)
#           define printLog(format, ...) printMessage(format, ## __VA_ARGS__);
#       endif
#   endif

#else

#   ifndef printTagLog
#       define printTagLog(tag, format, ...)  { }
#   endif

#   ifndef printLog
#       define printLog(format, ...) { }
#   endif

#endif


#ifdef __cplusplus
}
#endif


#endif

/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef __LOG_H
#define __LOG_H


#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <string.h>

#include "gtime.h"


#if defined(_DEBUG) || defined(DEBUG) || defined(GBEDUG_FORCE)

#	ifndef printMessage
#       if defined(__MINGW32__) || defined(_MSC_VER)
#       	define printMessage(format, ...) printf(format, ## __VA_ARGS__);
#   	elif defined(__GNUC__)
#       	define printMessage(format, ...) printf(format __VA_OPT__(,) __VA_ARGS__);
#   	else
#       	define printMessage(format, ...) { }
#       	pragma message("Please check compiler")
#   	endif
#	endif

#   ifndef printTagLog
#       ifdef USE_HAL_DRIVER
#			define __printTagLog_PARAM "lu"
#		else
#			define __printTagLog_PARAM "llu"
# 		endif
#       if defined(__MINGW32__) || defined(_MSC_VER)
#           define printTagLog(tag, format, ...) printMessage("%08" __printTagLog_PARAM "->%s:\t" format "\n", getMillis(), tag, ## __VA_ARGS__);
#       elif defined(__GNUC__)
#           define printTagLog(tag, format, ...) printMessage("%08" __printTagLog_PARAM "->%s:\t" format "\n", getMillis() % 10000000, tag __VA_OPT__(,) __VA_ARGS__);
#       endif
#   endif

#   ifndef printLog
#       if defined(__MINGW32__) || defined(_MSC_VER)
#           define printPretty(format, ...) printMessage("          \t" format, ## __VA_ARGS__);
#       elif defined(__GNUC__)
#           define printPretty(format, ...) printMessage("          \t" format __VA_OPT__(,) __VA_ARGS__);
#       endif
#   endif

#   ifndef gprint
#       if defined(__MINGW32__) || defined(_MSC_VER)
#           define gprint(format, ...) printMessage(format, ## __VA_ARGS__);
#       elif defined(__GNUC__)
#           define gprint(format, ...) printMessage(format __VA_OPT__(,) __VA_ARGS__); // TODO: __VA_OPT__ is not available until C++20
#       endif
#   endif

#else

#	ifndef printMessage
#		define printMessage(format, ...) { }
#	endif

#   ifndef printTagLog
#       define printTagLog(tag, format, ...)  { }
#   endif

#   ifndef printLog
#       define printPretty(format, ...) { }
#   endif

#   ifndef gprint
#       define gprint(format, ...) { }
#   endif

#endif


#ifdef __cplusplus
}
#endif


#endif

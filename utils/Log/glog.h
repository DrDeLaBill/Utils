/* Copyright © 2025 Georgy E. All rights reserved. */

#ifndef _GLOG_H_
#define _GLOG_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <string.h>

#include "gtime.h"
#include "bmacro.h"


#if defined(_DEBUG) || defined(DEBUG) || defined(GBEDUG_FORCE)

    int __gprint_tag_offset(const char* tag);

    #define __GLOG_MILLIS_SYMBOLS_CNT_ 8
    #define __GLOG_MILLIS_TIME_DIV_    100000000
    #define __GLOG_TOSTRING_(x)        __STR_DEF__(x)
    #define __GLOG_PRETTY_LOG_OFFSET_  (__GLOG_MILLIS_SYMBOLS_CNT_ + 8)

    #ifndef printMessage
       #if defined(__MINGW32__) || defined(_MSC_VER)
           #define printMessage(format, ...) printf(format,  ##__VA_ARGS__);
       #elif defined(__GNUC__)
            #if __cplusplus >= 202002L
                #define printMessage(format, ...) printf(format __VA_OPT__(,) __VA_ARGS__);
            #else
                #define printMessage(format, ...) printf(format, ##__VA_ARGS__);
            #endif
       #else
           #define printMessage(format, ...) { }
           #pragma message("Please check compiler")
       #endif
    #endif

    #ifndef printTagLog
        #if TIME_MS_T == uint32_t
            #define __printTagLog_PARAM "lu"
        #else
            #define __printTagLog_PARAM "llu"
        #endif
        #if defined(__MINGW32__) || defined(_MSC_VER)
            #define printTagLog(tag, format, ...) printMessage("%0" __GLOG_TOSTRING_(__GLOG_MILLIS_SYMBOLS_CNT_) __printTagLog_PARAM "->%s:%*s" format "\n", getMillis() % __GLOG_MILLIS_TIME_DIV_, tag, __gprint_tag_offset(tag), "", ##__VA_ARGS__);
        #elif defined(__GNUC__)
            #if __cplusplus >= 202002L
                #define printTagLog(tag, format, ...) printMessage("%0" __GLOG_TOSTRING_(__GLOG_MILLIS_SYMBOLS_CNT_) __printTagLog_PARAM "->%s:%*s" format "\n", getMillis() % __GLOG_MILLIS_TIME_DIV_, tag, __gprint_tag_offset(tag), "" __VA_OPT__(,) __VA_ARGS__);
            #else
                #define printTagLog(tag, format, ...) printMessage("%0" __GLOG_TOSTRING_(__GLOG_MILLIS_SYMBOLS_CNT_) __printTagLog_PARAM "->%s:%*s" format "\n", getMillis() % __GLOG_MILLIS_TIME_DIV_, tag, __gprint_tag_offset(tag), "", ##__VA_ARGS__);
            #endif
        #endif
   #endif

    #ifndef printLog
        #if defined(__MINGW32__) || defined(_MSC_VER)
            #define printPretty(format, ...) printMessage("%*s" format, __GLOG_PRETTY_LOG_OFFSET_, "", ##__VA_ARGS__);
        #elif defined(__GNUC__)
            #if __cplusplus >= 202002L
                #define printPretty(format, ...) printMessage("%*s" format, __GLOG_PRETTY_LOG_OFFSET_, "" __VA_OPT__(,) __VA_ARGS__);
            #else
                #define printPretty(format, ...) printMessage("%*s" format, __GLOG_PRETTY_LOG_OFFSET_, "", ##__VA_ARGS__);
            #endif
        #endif
    #endif

    #ifndef gprint
        #if defined(__MINGW32__) || defined(_MSC_VER)
            #define gprint(format, ...) printMessage(format, ## __VA_ARGS__);
        #elif defined(__GNUC__)
            #if __cplusplus >= 202002L
                #define gprint(format, ...) printMessage(format __VA_OPT__(,) __VA_ARGS__);
            #else
                #define gprint(format, ...) printMessage(format, ##__VA_ARGS__);
            #endif
        #endif
    #endif

#else

    #ifndef printMessage
        #define printMessage(format, ...) { }
    #endif

   #ifndef printTagLog
       #define printTagLog(tag, format, ...)  { }
   #endif

   #ifndef printLog
       #define printPretty(format, ...) { }
   #endif

   #ifndef gprint
       #define gprint(format, ...) { }
   #endif

#endif


#ifdef __cplusplus
}
#endif


#endif

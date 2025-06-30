/* Copyright © 2025 Georgy E. All rights reserved. */

#ifndef _GLOG_H_
#define _GLOG_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <string.h>

#include "gtime.h"


#if defined(_DEBUG) || defined(DEBUG) || defined(GBEDUG_FORCE)

    #if TIME_MS_T == uint32_t
        #define __G_TIME_PRINT_LEN "lu"
    #else
        #define __G_TIME_PRINT_LEN "llu"
    #endif

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
           #error Please check compiler
       #endif
    #endif

    void __g_print_offset();
    void __g_print_tag(const char* tag);

    #ifndef printTagLog
        #if defined(__MINGW32__) || defined(_MSC_VER)
            #define printTagLog(tag, format, ...)  { \
                __g_print_tag(tag); \
                printMessage(format "\n", ##__VA_ARGS__); \
            }
        #elif defined(__GNUC__)
            #if __cplusplus >= 202002L
                #define printTagLog(tag, format, ...)  { \
                    __g_print_tag(tag); \
                    printMessage(format "\n" __VA_OPT__(,) __VA_ARGS__); \
                }
            #else
                #define printTagLog(tag, format, ...)  { \
                    __g_print_tag(tag); \
                    printMessage(format "\n", ##__VA_ARGS__); \
                }
            #endif
        #endif
   #endif

    #ifndef printLog
        #if defined(__MINGW32__) || defined(_MSC_VER)
            #define printPretty(format, ...) { \
                __g_print_offset(); \
                printMessage(format, ##__VA_ARGS__); \
            }
        #elif defined(__GNUC__)
            #if __cplusplus >= 202002L
                #define printPretty(format, ...) { \
                    __g_print_offset(); \
                    printMessage(format __VA_OPT__(,) __VA_ARGS__); \
                }
            #else
                #define printPretty(format, ...) { \
                    __g_print_offset(); \
                    printMessage(format, ##__VA_ARGS__); \
                }
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

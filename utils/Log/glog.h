/* Copyright © 2025 Georgy E. All rights reserved. */

#ifndef _GLOG_H_
#define _GLOG_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>

#include "gtime.h"


#if defined(_DEBUG) || defined(DEBUG) || defined(GBEDUG_FORCE)

    #define GPRINT_ENABLED
    
    bool __g_print_msg_filter_check(const char*, uint32_t);
    #ifndef gprintMsgFilter
        #define gprintMsgFilter(PRINT, TIME) if (__g_print_msg_filter_check(__STR_DEF__(PRINT), TIME)) { PRINT; }
    #endif

#else

    #define gprintMsgFilter(PRINT, TIME) {}

#endif


void gprint(const char* format, ...);
void printTagLog(const char* TAG, const char* format, ...);
void printPretty(const char* format, ...);


#ifdef __cplusplus
}
#endif


#endif

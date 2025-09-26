/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef __G_TIME_H
#define __G_TIME_H


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>


#if defined(USE_HAL_DRIVER) || defined(ARDUINO)
    #define TIME_MS_T uint32_t 
#else
    #define TIME_MS_T uint64_t
#endif


TIME_MS_T getMillis();

uint64_t getMicroseconds();


#ifdef __cplusplus
}
#endif


#endif

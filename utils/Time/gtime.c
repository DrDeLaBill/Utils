/* Copyright © 2023 Georgy E. All rights reserved. */

#include "gtime.h"

#include <stdint.h>

#include "bmacro.h"


#if defined(USE_HAL_DRIVER)
#	include "hal_defs.h"
#elif defined(__GNUC__)
#   include <sys/time.h>
#elif defined(_MSC_VER)
#   include <time.h>
#else
#   pragma _WARNING("Please select the target STM32xxxx used in your application")
#endif


#ifdef USE_HAL_DRIVER
uint32_t getMillis()
#else
unsigned long long getMillis()
#endif
{
#if defined(USE_HAL_DRIVER)
    return HAL_GetTick();
#elif defined(__GNUC__)
    struct timeval time;
    gettimeofday(&time, NULL);
    return ((unsigned long long)(time.tv_sec) * 1000) + ((unsigned long long)(time.tv_usec) / 1000);
#elif defined(_MSC_VER)
    return clock();
#else
    return 0;
#endif
}

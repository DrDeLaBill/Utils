/* Copyright © 2023 Georgy E. All rights reserved. */

#include "gtime.h"

#include <stdint.h>

#include "bmacro.h"


#if defined(USE_HAL_DRIVER)
#elif defined(ARDUINO)
#   include <Arduino.h>
#elif defined(__GNUC__)
#   include <stddef.h>
#   include <sys/time.h>
#elif defined(_MSC_VER)
#   include <time.h>
#else
#   pragma _WARNING("Please select the target STM32xxxx used in your application")
#endif


TIME_MS_T getMillis()
{
#if defined(USE_HAL_DRIVER)
	extern uint32_t HAL_GetTick();
    return HAL_GetTick();
#elif defined(ARDUINO)
    return millis();
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

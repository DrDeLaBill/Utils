/* Copyright © 2023 Georgy E. All rights reserved. */

#include "gtime.h"

#include <stdint.h>

#include "bmacro.h"

#if defined(ARDUINO)
    #include <Arduino.h>
#elif defined(__GNUC__)
    #include <stddef.h>
    #include <sys/time.h>
#elif defined(_MSC_VER)
    #include <time.h>
#else
    #warning Please select the target STM32xxxx used in your application
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
    return ((TIME_MS_T)(time.tv_sec) * 1000) + ((TIME_MS_T)(time.tv_usec) / 1000);
#elif defined(_MSC_VER)
    return clock();
#else
    return 0;
#endif
}


uint64_t getMicroseconds()
{
#if defined(USE_HAL_DRIVER)
    #warning You need to initialize DWT
	extern uint32_t HAL_GetTick();
    return HAL_GetTick() * 1000; // TODO
#elif defined(ARDUINO)
    return micros();
#elif defined(__GNUC__)
    struct timeval time;
    gettimeofday(&time, NULL);
    return ((uint64_t)(time.tv_sec) * 1000000) + ((uint64_t)(time.tv_usec));
#elif defined(_MSC_VER)
    return (uint64_t)clock() * (1000000 / CLOCKS_PER_SEC);
#else
    return 0;
#endif
}
/* Copyright © 2023 Georgy E. All rights reserved. */

#include "gtime.h"

#include <stdint.h>

#include "bmacro.h"

#if defined(ARDUINO)
    #include <Arduino.h>
#elif defined(USE_HAL_DRIVER)
    #include "main.h"
#elif defined(__GNUC__)
    #include <stddef.h>
    #include <sys/time.h>
#elif defined(_MSC_VER)
    #include <time.h>
#else
    #warning Please select the target STM32xxxx used in your application
#endif


#define U64_MAX (4294967296ULL)


g_time_t getMillis()
{
#if defined(USE_HAL_DRIVER)
    static g_time_t previous_ticks = 0;
    static g_time_t overflow_count = 0;

    g_time_t current_ticks = HAL_GetTick();

    if (current_ticks < previous_ticks) {
        overflow_count++;
    }
    previous_ticks = current_ticks;

    return (overflow_count * U64_MAX) + current_ticks;
#elif defined(ARDUINO)
    static g_time_t previous_ticks = 0;
    static g_time_t overflow_count = 0;

    g_time_t current_ticks = millis();

    if (current_ticks < previous_ticks) {
        overflow_count++;
    }
    previous_ticks = current_ticks;

    return (overflow_count * U64_MAX) + current_ticks;
#elif defined(__GNUC__)
    struct timeval time;
    gettimeofday(&time, NULL);
    return ((g_time_t)(time.tv_sec) * 1000) + ((g_time_t)(time.tv_usec) / 1000);
#elif defined(_MSC_VER)
    return clock();
#else
    return 0;
#endif
}

g_time_t getMicrosecondes()
{
#if defined(USE_HAL_DRIVER) || defined(ARDUINO)
    g_time_t ticks   = getMillis();
    g_time_t systick = SysTick->VAL;
    g_time_t load    = SysTick->LOAD;

    g_time_t micros = (g_time_t)ticks * 1000ULL;
    micros += (load - systick) / (SystemCoreClock / 1000000ULL);

    return micros;
#elif defined(__GNUC__)
    struct timeval time;
    gettimeofday(&time, NULL);
    return ((g_time_t)(time.tv_sec) * 1000000) + ((g_time_t)(time.tv_usec));
#elif defined(_MSC_VER)
    return (v)clock() * (1000000 / CLOCKS_PER_SEC);
#else
    return 0;
#endif
}

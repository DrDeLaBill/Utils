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


#define U32_MAX ((g_time_t)0xFFFFFFFF)


g_time_t getMillis()
{
#if defined(USE_HAL_DRIVER)
    static g_time_t previous_ticks = 0;
    static g_time_t overflow_count = 0;

    __disable_irq();
    g_time_t current_ticks = HAL_GetTick();
    if (current_ticks < previous_ticks) {
        overflow_count++;
    }
    previous_ticks = current_ticks;
    __enable_irq();

    return (overflow_count * U32_MAX) + current_ticks;
#elif defined(ARDUINO)
    static g_time_t previous_ticks = 0;
    static g_time_t overflow_count = 0;
    static bool irq_disabled = false;
    bool _self_irq_disabled = false;

    uint32_t prev_basepri = __get_BASEPRI();
    if (!irq_disabled) {
        irq_disabled = true;
        _self_irq_disabled = true;
        __set_BASEPRI(0x40);
    }

    g_time_t current_ticks = millis();
    if (current_ticks < previous_ticks) {
        overflow_count++;
    }
    previous_ticks = current_ticks;

    if (_self_irq_disabled) {
        irq_disabled = false;
        __set_BASEPRI(prev_basepri);
    }

    return (overflow_count * U32_MAX) + current_ticks;
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

g_time_t getMicroseconds()
{
#if defined(USE_HAL_DRIVER)
    g_time_t ticks   = getMillis();
    g_time_t systick = SysTick->VAL;
    g_time_t load    = SysTick->LOAD;

    g_time_t micros = (g_time_t)ticks * 1000ULL;
    micros += (load - systick) / (SystemCoreClock / 1000000ULL);

    return micros;
#elif defined(ARDUINO)
    static g_time_t previous_ticks = 0;
    static g_time_t overflow_count = 0;
    static bool irq_disabled = false;
    bool _self_irq_disabled = false;

    uint32_t prev_basepri = __get_BASEPRI();
    if (!irq_disabled) {
        irq_disabled = true;
        _self_irq_disabled = true;
        __set_BASEPRI(0x40);
    }

    g_time_t current_ticks = micros();
    if (current_ticks < previous_ticks) {
        overflow_count++;
    }
    previous_ticks = current_ticks;

    if (_self_irq_disabled) {
        irq_disabled = false;
        __set_BASEPRI(prev_basepri);
    }

    return (overflow_count * U32_MAX) + current_ticks;
#elif defined(__GNUC__)
    struct timeval time;
    gettimeofday(&time, NULL);
    return ((g_time_t)(time.tv_sec) * 1000000) + ((g_time_t)(time.tv_usec));
#elif defined(_MSC_VER)
    return (g_time_t)clock() * (1000000 / CLOCKS_PER_SEC);
#else
    return 0;
#endif
}

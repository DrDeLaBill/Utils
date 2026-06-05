/* Copyright © 2026 Georgy E. All rights reserved. */

#include "gtime.h"

#include <stdint.h>

#include "bmacro.h"

#if defined(__ZEPHYR__)
    #include <zephyr/kernel.h>
#elif defined(ESP_PLATFORM)
    #include "esp_timer.h"
#elif defined(USE_HAL_DRIVER)
    #include "main.h"
#elif defined(ARDUINO)
    #include <Arduino.h>
#elif defined(INC_FREERTOS_H) || defined(FREERTOS)
    #include "FreeRTOS.h"
    #include "task.h"
#elif defined(__GNUC__) && !defined(__arm__)
    #include <sys/time.h>
    #include <stddef.h>
#elif defined(_MSC_VER)
    #include <time.h>
#else
    #warning Please select the target STM32xxxx used in your application
#endif


__attribute__((weak)) uint32_t getMillis()
{
#if defined(__ZEPHYR__)

    return k_uptime_get_32();
    
#elif defined(ESP_PLATFORM)

    return (uint32_t)(esp_timer_get_time() / 1000ULL);
    
#elif defined(INC_FREERTOS_H) || defined(FREERTOS)

    return (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
    
#elif defined(USE_HAL_DRIVER)

    return HAL_GetTick();
    
#elif defined(ARDUINO)

    return millis();
    
#elif defined(__GNUC__) && !defined(__arm__)

    struct timeval time;
    gettimeofday(&time, NULL);
    return (uint32_t)(((time.tv_sec) * 1000) + ((time.tv_usec) / 1000));
    
#elif defined(_MSC_VER)

    return (uint32_t)(clock() * (1000 / CLOCKS_PER_SEC));
    
#else

    return 0;

#endif
}

uint64_t getMillis64bit()
{
#if defined(__ZEPHYR__)

    return (uint64_t)k_uptime_get();

#elif defined(ESP_PLATFORM)

    return (uint64_t)(esp_timer_get_time() / 1000ULL);

#elif defined(USE_HAL_DRIVER)

    static uint32_t previous_ticks = 0;
    static uint32_t overflow_count = 0;

    __disable_irq();
    uint32_t current_ticks = HAL_GetTick();
    if (current_ticks < previous_ticks) {
        overflow_count++;
    }
    previous_ticks = current_ticks;
    __enable_irq();

    return (((uint64_t)overflow_count) << 32) | current_ticks;

#elif defined(ARDUINO)

    static uint32_t previous_ticks = 0;
    static uint32_t overflow_count = 0;

    noInterrupts();
    uint32_t current_ticks = millis();
    if (current_ticks < previous_ticks) {
        overflow_count++;
    }
    previous_ticks = current_ticks;
    interrupts();

    return (((uint64_t)overflow_count) << 32) | current_ticks;

#elif defined(INC_FREERTOS_H) || defined(FREERTOS)

    static uint32_t previous_ticks = 0;
    static uint32_t overflow_count = 0;

    taskENTER_CRITICAL();
    uint32_t current_ticks = (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
    if (current_ticks < previous_ticks) {
        overflow_count++;
    }
    previous_ticks = current_ticks;
    taskEXIT_CRITICAL();

    return (((uint64_t)overflow_count) << 32) | current_ticks;

#elif defined(__GNUC__) && !defined(__arm__)

    struct timeval time;
    gettimeofday(&time, NULL);
    return ((uint64_t)(time.tv_sec) * 1000ULL) + ((uint64_t)(time.tv_usec) / 1000ULL);

#elif defined(_MSC_VER)

    return (uint64_t)clock();

#else

    return 0;

#endif
}

__attribute__((weak)) uint64_t getMicroseconds()
{
#if defined(__ZEPHYR__)

    return (uint64_t)k_ticks_to_us_floor64(sys_clock_tick_get());

#elif defined(ESP_PLATFORM)

    return (uint64_t)esp_timer_get_time();

#elif defined(USE_HAL_DRIVER)

    uint32_t m0, m1, systick, load;
    
    do {
        m0 = HAL_GetTick();
        systick = SysTick->VAL;
        load = SysTick->LOAD;
        m1 = HAL_GetTick();
    } while (m0 != m1);

    uint64_t micros = (uint64_t)m0 * 1000ULL;
    micros += (load - systick) / (SystemCoreClock / 1000000ULL);

    return micros;

#elif defined(ARDUINO)

    static uint32_t previous_ticks = 0;
    static uint32_t overflow_count = 0;

    noInterrupts();
    uint32_t current_ticks = micros();
    if (current_ticks < previous_ticks) {
        overflow_count++;
    }
    previous_ticks = current_ticks;
    interrupts();

    return (((uint64_t)overflow_count) << 32) | current_ticks;

#elif defined(__GNUC__) && !defined(__arm__)

    struct timeval time;
    gettimeofday(&time, NULL);
    return ((uint64_t)(time.tv_sec) * 1000000ULL) + ((uint64_t)(time.tv_usec));

#elif defined(_MSC_VER)

    return (uint64_t)clock() * (1000000ULL / CLOCKS_PER_SEC);

#else

    return 0;

#endif
}

void gtimer_start(gtimer_t* tm, uint32_t delay) {
	tm->start = getMillis();
	tm->delay = delay;
}

void gtimer_reload(gtimer_t* tm)
{
	tm->start = getMillis();
}

bool gtimer_wait(gtimer_t* tm) {
    return (getMillis() - tm->start) < tm->delay;
}

void gtimer_reset(gtimer_t* tm)
{
	tm->delay = 0;
	tm->start = 0;
}
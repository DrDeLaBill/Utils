/* Copyright © 2023 Georgy E. All rights reserved. */

#include "gtime.h"

#include <stdint.h>

#include "bmacro.h"


#if defined(STM32F100xB) || \
    defined(STM32F100xE) || \
    defined(STM32F101x6) || \
    defined(STM32F101xB) || \
    defined(STM32F101xE) || \
    defined(STM32F101xG) || \
    defined(STM32F102x6) || \
    defined(STM32F102xB) || \
    defined(STM32F103x6) || \
    defined(STM32F103xB) || \
    defined(STM32F103xE) || \
    defined(STM32F103xG) || \
    defined(STM32F105xC) || \
    defined(STM32F107xC)
#   include "stm32f1xx_hal.h"
#   define _HAL_TIME
#elif defined(STM32F405xx) || \
    defined(STM32F415xx) || \
    defined(STM32F407xx) || \
    defined(STM32F417xx) || \
    defined(STM32F427xx) || \
    defined(STM32F437xx) || \
    defined(STM32F429xx) || \
    defined(STM32F439xx) || \
    defined(STM32F401xC) || \
    defined(STM32F401xE) || \
    defined(STM32F410Tx) || \
    defined(STM32F410Cx) || \
    defined(STM32F410Rx) || \
    defined(STM32F411xE) || \
    defined(STM32F446xx) || \
    defined(STM32F469xx) || \
    defined(STM32F479xx) || \
    defined(STM32F412Cx) || \
    defined(STM32F412Zx) || \
    defined(STM32F412Rx) || \
    defined(STM32F412Vx) || \
    defined(STM32F413xx) || \
    defined(STM32F423xx)
#   include "stm32f4xx_hal.h"
#   define _HAL_TIME
#elif defined(__GNUC__)
#   include <sys/time.h>
#   pragma _WARNING("please select the target STM32xxxx used in your application")
#elif defined(_MSC_VER)
#   include <time.h>
#   pragma _WARNING("please select the target STM32xxxx used in your application")
#else
#   pragma _WARNING("please select the target STM32xxxx used in your application")
#endif


uint32_t getMillis()
{
#if defined(_HAL_TIME)
    return HAL_GetTick();
#elif defined(__GNUC__)
    struct timeval time;
    gettimeofday(&time, NULL);
    return time.tv_sec * 1000 + time.tv_usec / 1000;
#elif defined(_MSC_VER)
    return clock();
#else
    return 0;
#endif
}

#ifdef _HAL_TIME
#   undef _HAL_TIME
#endif

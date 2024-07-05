/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef _HAL_DEFS_H_
#define _HAL_DEFS_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>


#if defined(USE_HAL_DRIVER)
#	if defined(STM32F100xB) || \
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
#		include "stm32f1xx_hal.h"
#	elif defined(STM32F2)
#		include "stm32f2xx_hal.h"
#	elif defined(STM32F3)
#		include "stm32f3xx_hal.h"
#   elif defined(STM32F405xx) || \
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

#		include "stm32f4xx_hal.h"
#	else
#		error "Please select the target STM32Fxxx used in your application"
#	endif


#   define STM_REF_VOLTAGEx10        (12)
#   define STM_MIN_VOLTAGEx10        (20)
#   define STM_MAX_VOLTAGEx10        (36)

#   define STM_ADC_MAX               ((uint32_t)0xFFF)


typedef struct _GPIO_PAIR {
	GPIO_TypeDef* port;
	uint16_t      pin;
} GPIO_PAIR;


void SystemInfo(void);
bool MCUcheck(void);


#endif


#define TIMESTAMP2000_01_01_00_00_00 (946670400)

#define SECOND_MS                    ((uint32_t)(1000))

#define MINUTE_S                     ((uint32_t)(60))
#define MINUTE_MS                    ((uint32_t)(MINUTE_S * SECOND_MS))

#define HOUR_MIN                     ((uint32_t)(60))
#define HOUR_MS                      ((uint32_t)(HOUR_MIN * MINUTE_MS))

#define DAY_H                        ((uint32_t)(24))
#define DAY_MS                       ((uint32_t)(DAY_H * HOUR_MS))

#define WEEK_D                       ((uint32_t)(7))
#define WEEK_MS                      ((uint32_t)(WEEK_D * DAY_MS))

#define BITS_IN_BYTE                 (8)


#ifdef __cplusplus
}
#endif


#endif

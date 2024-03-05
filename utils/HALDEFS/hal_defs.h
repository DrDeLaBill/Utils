/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef _HAL_DEFS_H_
#define _HAL_DEFS_H_


#include <stdint.h>


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
#	elif defined(STM32F405xx) || \
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
#		error "Please select the target STM32xxxx used in your application"
#	endif
#endif


#define ADC_MAX                      ((uint32_t)0xFFF)

#define TIMESTAMP2000_01_01_00_00_00 (946670400)

#define SECOND_MS                    ((uint32_t)(1000))

#define MINUTE_MS                    ((uint32_t)(60 * SECOND_MS))

#define HOUR_MS                      ((uint32_t)(60 * MINUTE_MS))

#define DAY_MS                       ((uint32_t)(24 * HOUR_MS))

#define WEEK_MS                      ((uint32_t)(7 * DAY_MS))

#define BITS_IN_BYTE                 ((uint8_t)8)


#endif

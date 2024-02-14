/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef _UTILS_H_
#define _UTILS_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>

#include "hal_defs.h"


#ifndef __abs
#	define __abs(num1) (((num1) > 0) ? (num1) : ((num1) * -1))
#endif

#ifndef __abs_dif
#	define __abs_dif(num1, num2) (((num1) > (num2)) ? (num1) - (num2) : (num2) - (num1))
#endif

#ifndef __arr_len
#    define __arr_len(arr) (sizeof(arr) / sizeof(*arr))
#endif

#ifndef __div_up
#	define __div_up(num, div) (((num) / (div)) + ((num) % (div) ? 1 : 0))
#endif

#ifndef __min
#   define __min(num1, num2) ((num1) < (num2) ? (num1) : (num2))
#endif

#ifndef __max
#   define __max(num1, num2) ((num1) > (num2) ? (num1) : (num2))
#endif


typedef struct _util_timer_t {
    uint32_t start;
    uint32_t delay;
} util_old_timer_t;


void     util_old_timer_start(util_old_timer_t* tm, uint32_t waitMs);
bool     util_old_timer_wait(util_old_timer_t* tm);


typedef struct _util_port_pin_t {
    GPIO_TypeDef* port;
    uint16_t      pin;
} util_port_pin_t;


void     util_debug_hex_dump(const uint8_t* buf, uint32_t start_counter, uint16_t len);
bool     util_wait_event(bool (*condition) (void), uint32_t time);
uint8_t  util_get_number_len(int number);
uint32_t util_small_pow(const uint32_t number, uint32_t degree);


#ifdef __cplusplus
}
#endif


#endif

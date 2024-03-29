/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef _UTILS_H_
#define _UTILS_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>

#include "bmacro.h"


#ifdef USE_HAL_DRIVER
#   include "hal_defs.h"
#endif


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

#ifndef __get_bit
#	define __get_bit(REG, NUM) (((REG) >> (NUM)) & (uint64_t)1)
#endif

#ifndef __set_bit
#	define __set_bit(REG, NUM) ((REG) |= ((uint64_t)1 << (NUM)))
#endif

#ifndef __reset_bit
#	define __reset_bit(REG, NUM) ((REG) &= ~((uint64_t)1 << (NUM)))
#endif


#ifndef TYPE_PACK
#   ifdef __MINGW32__
#       define TYPE_PACK( __Type__, __Declaration__ ) // TODO
#   elif defined(__GNUC__)
#       define TYPE_PACK( __Type__, __Declaration__ )  __Type__ __attribute__((__packed__, aligned(1))) __Declaration__
#   elif defined(_MSC_VER)
#       define TYPE_PACK( __Type__, __Declaration__ ) __pragma(pack(push, 1) ) __Type__ __Declaration__ __pragma(pack(pop))
#   endif
#endif


typedef struct _util_timer_t {
    uint32_t start;
    uint32_t delay;
} util_old_timer_t;


void     util_old_timer_start(util_old_timer_t* tm, uint32_t waitMs);
bool     util_old_timer_wait(util_old_timer_t* tm);


#ifdef USE_HAL_DRIVER
typedef struct _util_port_pin_t {
    GPIO_TypeDef* port;
    uint16_t      pin;
} util_port_pin_t;
#endif


void     util_debug_hex_dump(const uint8_t* buf, uint32_t start_counter, uint16_t len);
bool     util_wait_event(bool (*condition) (void), uint32_t time);
uint8_t  util_get_number_len(int number);
uint32_t util_small_pow(const uint32_t number, uint32_t degree);
int      util_convert_range(int val, int rngl1, int rngh1, int rngl2, int rngh2);


#ifdef __cplusplus
}
#endif


#endif

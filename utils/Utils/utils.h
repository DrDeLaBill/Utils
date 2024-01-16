/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef __UTILS_H
#define __UTILS_H


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>


#ifndef __abs_dif
#	define __abs_dif(num1, num2) (((num1) > (num2)) ? (num1) - (num2) : (num2) - (num1))
#endif

#ifndef __arr_len
#    define __arr_len(arr) (sizeof(arr) / sizeof(*arr))
#endif


void     util_debug_hex_dump(const uint8_t* buf, uint32_t start_counter, uint16_t len);
bool     util_wait_event(bool (*condition) (void), uint32_t time);
uint8_t  util_get_number_len(int number);


#ifdef __cplusplus
}
#endif


#endif

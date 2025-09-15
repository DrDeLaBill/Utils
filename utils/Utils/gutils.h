/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef _UTILS_H_
#define _UTILS_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "glog.h"
#include "gtime.h"
#include "bmacro.h"


#ifndef __abs
#    define __abs(num1) (((num1) > 0) ? (num1) : ((num1) * -1))
#endif

#ifndef __abs_dif
#    define __abs_dif(num1, num2) (((num1) > (num2)) ? (num1) - (num2) : (num2) - (num1))
#endif

#ifndef __arr_len
#    define __arr_len(arr) (sizeof(arr) / sizeof(*arr))
#endif

#ifndef __div_up
#    define __div_up(num, div) (((num) / (div)) + ((num) % (div) ? 1 : 0))
#endif

#ifndef __min
#   define __min(num1, num2) ((num1) < (num2) ? (num1) : (num2))
#endif

#ifndef __max
#   define __max(num1, num2) ((num1) > (num2) ? (num1) : (num2))
#endif

#ifndef __get_bit
#    define __get_bit(REG, NUM) (((REG) >> (NUM)) & 0x1UL)
#endif

#ifndef __set_bit
#    define __set_bit(REG, NUM) ((REG) |= (0x1UL << (NUM)))
#endif

#ifndef __reset_bit
#    define __reset_bit(REG, NUM) ((REG) &= ~(0x1UL << (NUM)))
#endif

#ifndef __concat
#   define __concat(a, b) a##b
#endif

#ifndef __proportion
#   define __proportion(VAL, RNG1L, RNG1H, RNG2L, RNG2H) ( \
        (__abs_dif(RNG1H, RNG1L) > 0) ? \
            (RNG2L + ((__abs_dif(RNG1L, VAL) * __abs_dif(RNG2H, RNG2L)) / __abs_dif(RNG1H, RNG1L))) : \
            0 \
)
#endif

#ifndef __proportion_inv
#   define __proportion_inv(VAL, RNG1L, RNG1H, RNG2L, RNG2H) ( \
        (__abs_dif(RNG1H, RNG1L) > 0) ? \
            (RNG2L + (__abs_dif(RNG2L, RNG2H) - ((__abs_dif(RNG1L, VAL) * __abs_dif(RNG2H, RNG2L)) / __abs_dif(RNG1H, RNG1L)))) : \
            0 \
)
#endif

#ifndef __percent
#   define __percent(PART, MAX) ( MAX > 0 ? ((PART) * 100) / (MAX) : 0)
#endif

#ifndef __rm_mod
#   define __rm_mod(NUM, DIV) ((NUM) - ((NUM) % (DIV)))
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


#ifndef __define_TPC
#   define __define_TPC(NAME) { \
                                  static uint32_t __concat(NAME, _cnt) = 0; \
                                  static uint32_t __concat(NAME, _ms)  = getMillis(); \
                                  static char __concat(NAME, _str)[]   = #NAME; \
                                  if (getMillis() - __concat(NAME, _ms) > SECOND_MS) { \
                                      printTagLog("TPC", "%s->%08lu", __concat(NAME, _str), __concat(NAME, _cnt)); \
                                      __concat(NAME, _cnt) = 0; \
                                      __concat(NAME, _ms)  = getMillis(); \
                                  } else { \
                                      __concat(NAME, _cnt)++; \
                                  } \
                              }
#endif


typedef struct _gtimer_t {
    TIME_MS_T start;
    TIME_MS_T delay;
} gtimer_t;


void gtimer_start(gtimer_t* tm, TIME_MS_T waitMs);
void gtimer_reload(gtimer_t* tm);
bool gtimer_wait(gtimer_t* tm);
uint32_t gtimer_remaining(gtimer_t* tm);
void gtimer_reset(gtimer_t* tm);


typedef enum _ALIGN_MODE {
    ALIGN_MODE_LEFT = 1,
    ALIGN_MODE_RIGHT,
    ALIGN_MODE_CENTER
} ALIGN_MODE;


#define IS_ALIGN_MODE(MODE) ((MODE) == ALIGN_MODE_LEFT || (MODE) == ALIGN_MODE_RIGHT || (MODE) == ALIGN_MODE_CENTER)



void     util_debug_hex_dump(const uint8_t* buf, uint32_t start_counter, uint16_t len);
bool     util_wait_event(bool (*condition) (void), TIME_MS_T time);
uint8_t  util_get_number_len(int number);
uint32_t util_small_pow(uint32_t number, uint32_t degree);
int      util_convert_range(int val, int rngl1, int rngh1, int rngl2, int rngh2);
unsigned util_hash(const uint8_t* data, const unsigned size);
uint8_t* util_memfind(uint8_t* buf, const size_t buf_size, const uint8_t* pattern, const size_t pattern_size);



#ifdef __cplusplus
}
#endif


#endif

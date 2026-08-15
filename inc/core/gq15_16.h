/* Copyright © 2026 Georgy E. All rights reserved. */

#ifndef _GQ15_16_H_
#define _GQ15_16_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>


#define GQ15_16_FRACTIONAL_BITS (16u)
#define GQ15_16_SCALE           (65536)


typedef int32_t gq15_16_t;


gq15_16_t gq15_16_from_raw(int32_t raw);
int32_t    gq15_16_raw(gq15_16_t value);

gq15_16_t gq15_16_from_int(int32_t value);
int32_t    gq15_16_to_int(gq15_16_t value);

gq15_16_t gq15_16_from_float(float value);
float      gq15_16_to_float(gq15_16_t value);

gq15_16_t gq15_16_add(gq15_16_t lhs, gq15_16_t rhs);
gq15_16_t gq15_16_sub(gq15_16_t lhs, gq15_16_t rhs);
gq15_16_t gq15_16_mul(gq15_16_t lhs, gq15_16_t rhs);
gq15_16_t gq15_16_div(gq15_16_t lhs, gq15_16_t rhs);


#ifdef __cplusplus
}
#endif


#endif
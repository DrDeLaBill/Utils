/* Copyright © 2026 Georgy E. All rights reserved. */

#include "gq15_16.h"

#include <limits.h>
#include <math.h>


static gq15_16_t _gq15_16_saturate(int64_t value)
{
    if (value > INT32_MAX) {
        return INT32_MAX;
    }
    if (value < INT32_MIN) {
        return INT32_MIN;
    }
    return (gq15_16_t)value;
}

static int64_t _gq15_16_round_div(int64_t numerator, int64_t denominator)
{
    int64_t quotient = numerator / denominator;
    int64_t remainder = numerator % denominator;
    int64_t abs_remainder = remainder < 0 ? -remainder : remainder;
    int64_t abs_denominator = denominator < 0 ? -denominator : denominator;

    if (abs_remainder * 2 >= abs_denominator) {
        quotient += (numerator < 0) != (denominator < 0) ? -1 : 1;
    }
    return quotient;
}

gq15_16_t gq15_16_from_raw(int32_t raw)
{
    return raw;
}

int32_t gq15_16_raw(gq15_16_t value)
{
    return value;
}

gq15_16_t gq15_16_from_int(int32_t value)
{
    return _gq15_16_saturate((int64_t)value * GQ15_16_SCALE);
}

int32_t gq15_16_to_int(gq15_16_t value)
{
    return value / GQ15_16_SCALE;
}

gq15_16_t gq15_16_from_float(float value)
{
    if (isnan(value)) {
        return 0;
    }
    if (value >= (float)INT32_MAX / GQ15_16_SCALE) {
        return INT32_MAX;
    }
    if (value <= (float)INT32_MIN / GQ15_16_SCALE) {
        return INT32_MIN;
    }
    return (gq15_16_t)(value * GQ15_16_SCALE + (value < 0.0f ? -0.5f : 0.5f));
}

float gq15_16_to_float(gq15_16_t value)
{
    return (float)value / GQ15_16_SCALE;
}

gq15_16_t gq15_16_add(gq15_16_t lhs, gq15_16_t rhs)
{
    return _gq15_16_saturate((int64_t)lhs + rhs);
}

gq15_16_t gq15_16_sub(gq15_16_t lhs, gq15_16_t rhs)
{
    return _gq15_16_saturate((int64_t)lhs - rhs);
}

gq15_16_t gq15_16_mul(gq15_16_t lhs, gq15_16_t rhs)
{
    return _gq15_16_saturate(_gq15_16_round_div((int64_t)lhs * rhs, GQ15_16_SCALE));
}

gq15_16_t gq15_16_div(gq15_16_t lhs, gq15_16_t rhs)
{
    if (rhs == 0) {
        return lhs < 0 ? INT32_MIN : (lhs > 0 ? INT32_MAX : 0);
    }
    return _gq15_16_saturate(_gq15_16_round_div((int64_t)lhs * GQ15_16_SCALE, rhs));
}
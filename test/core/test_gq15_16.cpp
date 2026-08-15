/* Copyright © 2026 Georgy E. All rights reserved. */

#include <gtest/gtest.h>

#include <climits>

#include "gq15_16.h"


TEST(Gq15_16, preserves_raw_value)
{
    gq15_16_t value = gq15_16_from_raw(-98304);

    ASSERT_EQ(gq15_16_raw(value), -98304);
    ASSERT_FLOAT_EQ(gq15_16_to_float(value), -1.5f);
}

TEST(Gq15_16, converts_float_with_nearest_rounding)
{
    const float resolution = 1.0f / GQ15_16_SCALE;

    ASSERT_FLOAT_EQ(gq15_16_to_float(gq15_16_from_float(12.25f)), 12.25f);
    ASSERT_FLOAT_EQ(gq15_16_to_float(gq15_16_from_float(-3.5f)), -3.5f);
    ASSERT_FLOAT_EQ(gq15_16_to_float(gq15_16_from_float(0.5f * resolution)), resolution);
    ASSERT_FLOAT_EQ(gq15_16_to_float(gq15_16_from_float(-0.5f * resolution)), -resolution);
}

TEST(Gq15_16, converts_integers_and_saturates_out_of_range_values)
{
    ASSERT_EQ(gq15_16_to_int(gq15_16_from_int(-42)), -42);
    ASSERT_EQ(gq15_16_raw(gq15_16_from_int(50000)), INT32_MAX);
    ASSERT_EQ(gq15_16_raw(gq15_16_from_int(-50000)), INT32_MIN);
    ASSERT_EQ(gq15_16_raw(gq15_16_from_float(40000.0f)), INT32_MAX);
    ASSERT_EQ(gq15_16_raw(gq15_16_from_float(-40000.0f)), INT32_MIN);
}

TEST(Gq15_16, performs_saturating_arithmetic)
{
    gq15_16_t one_and_half = gq15_16_from_float(1.5f);
    gq15_16_t two = gq15_16_from_int(2);

    ASSERT_FLOAT_EQ(gq15_16_to_float(gq15_16_add(one_and_half, two)), 3.5f);
    ASSERT_FLOAT_EQ(gq15_16_to_float(gq15_16_sub(one_and_half, two)), -0.5f);
    ASSERT_FLOAT_EQ(gq15_16_to_float(gq15_16_mul(one_and_half, two)), 3.0f);
    ASSERT_FLOAT_EQ(gq15_16_to_float(gq15_16_div(one_and_half, two)), 0.75f);
    ASSERT_EQ(gq15_16_raw(gq15_16_add(INT32_MAX, 1)), INT32_MAX);
    ASSERT_EQ(gq15_16_raw(gq15_16_sub(INT32_MIN, 1)), INT32_MIN);
}

TEST(Gq15_16, division_by_zero_saturates_by_sign)
{
    ASSERT_EQ(gq15_16_raw(gq15_16_div(gq15_16_from_int(1), 0)), INT32_MAX);
    ASSERT_EQ(gq15_16_raw(gq15_16_div(gq15_16_from_int(-1), 0)), INT32_MIN);
    ASSERT_EQ(gq15_16_raw(gq15_16_div(0, 0)), 0);
}
/* Copyright © 2026 Georgy E. All rights reserved. */

#include <gtest/gtest.h>

#include <stdint.h>

#include "filters.h"


TEST(FilterEmaTest, InitializesWithZeroState)
{
    filter_ema_t filter;

    ASSERT_TRUE(filter_ema_init(&filter, 3));
    ASSERT_EQ(filter.K, 3);
    ASSERT_EQ(filter.acc, 0);
    ASSERT_EQ(filter_ema_get(&filter), 0);
}

TEST(FilterEmaTest, RejectsInvalidInitialization)
{
    filter_ema_t filter;

    ASSERT_FALSE(filter_ema_init(nullptr, 3));
    ASSERT_FALSE(filter_ema_init(&filter, 31));
}

TEST(FilterEmaTest, ConvergesToConstantInput)
{
    filter_ema_t filter;
    ASSERT_TRUE(filter_ema_init(&filter, 2));

    for (int i = 0; i < 40; ++i) {
        filter_ema_update(&filter, 100);
    }

    ASSERT_EQ(filter_ema_get(&filter), 100);
}

TEST(FilterEmaTest, AppliesExpectedIntegerSmoothing)
{
    filter_ema_t filter;
    ASSERT_TRUE(filter_ema_init(&filter, 2));

    filter_ema_update(&filter, 100);
    filter_ema_update(&filter, 100);
    filter_ema_update(&filter, 100);

    ASSERT_EQ(filter_ema_get(&filter), 58);
}

TEST(FilterEmaTest, SupportsImmediateResponseWithZeroShift)
{
    filter_ema_t filter;
    ASSERT_TRUE(filter_ema_init(&filter, 0));

    filter_ema_update(&filter, 1234);

    ASSERT_EQ(filter_ema_get(&filter), 1234);
}

TEST(FilterEmaTest, SaturatesAccumulator)
{
    filter_ema_t filter;
    ASSERT_TRUE(filter_ema_init(&filter, 1));

    filter_ema_update(&filter, INT32_MAX);
    filter_ema_update(&filter, INT32_MAX);

    ASSERT_EQ(filter.acc, INT32_MAX);
    ASSERT_EQ(filter_ema_get(&filter), INT32_MAX / 2);
}
/* Copyright © 2026 Georgy E. All rights reserved. */

#include <gtest/gtest.h>

#include "gtime.h"


TEST(GtimeTest, MillisIsNonDecreasing)
{
    uint32_t t1 = getMillis();
    uint32_t t2 = getMillis();
    ASSERT_GE(t2, t1);
}


TEST(GtimeTest, MicrosIsNonDecreasing)
{
    uint64_t t1 = getMicroseconds();
    uint64_t t2 = getMicroseconds();
    ASSERT_GE(t2, t1);
}


TEST(GtimeTest, Millis64IsNotLessThanMillis32)
{
    uint64_t ms64 = getMillis64bit();
    uint32_t ms32 = getMillis();
    ASSERT_GE(ms64, (uint64_t)ms32);
}

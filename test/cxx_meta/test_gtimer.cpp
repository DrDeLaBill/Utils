/* Copyright © 2026 Georgy E. All rights reserved. */

#include <gtest/gtest.h>

#include "gtimer.hpp"


TEST(GTimerCppTest, DefaultStateIsNotStarted)
{
    utl::GTimer tm;

    ASSERT_FALSE(tm.wait());
    ASSERT_EQ(tm.deadline(), 0u);
    ASSERT_EQ(tm.passed(), 0u);
    ASSERT_EQ(tm.getStart(), 0u);
    ASSERT_EQ(tm.end(), 0u);
}


TEST(GTimerCppTest, StartChangeDelayReset)
{
    utl::GTimer tm(1000);

    tm.start();
    ASSERT_EQ(tm.getDelay(), 1000u);

    tm.changeDelay(2000);
    ASSERT_EQ(tm.getDelay(), 2000u);

    tm.reset();
    ASSERT_FALSE(tm.wait());
    ASSERT_EQ(tm.deadline(), 0u);
}

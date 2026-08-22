/* Copyright © 2026 Georgy E. All rights reserved. */

#include <gtest/gtest.h>

#include <cmath>

#include "gpid.hpp"


TEST(GpidTest, OutputIsClampedByLimits)
{
    GPID pid(100.0f, 10.0f, 0.0f);
    pid.setGains(1000.0f, 0.0f, 0.0f);

    float out = pid.update_f(0.0f, 100.0f);

    ASSERT_TRUE(std::isfinite(out));
    ASSERT_LE(std::abs(out), 100.0f);
}


TEST(GpidTest, ResetAndDebugAreSafe)
{
    GPID pid(50.0f, 0.0f, 0.1f);
    pid.setGains(1.0f, 1.0f, 1.0f);

    (void)pid.update_f(1.0f, 2.0f);
    pid.reset();
    pid.setDebugEnabled(true, 1);
    pid.show();

    float out = pid.update_f(2.0f, 2.0f);
    ASSERT_TRUE(std::isfinite(out));
}

TEST(GpidTest, FixedPointUpdateReturnsFixedPointOutput)
{
    GPID pid(100.0f, 10.0f, 0.0f);
    pid.setGains(2.0f, 0.0f, 0.0f);

    gq15_16_t out = pid.update_q15_16(gq15_16_from_int(0), gq15_16_from_int(10));

    ASSERT_EQ(gq15_16_to_int(out), 28);
}

TEST(GpidTest, FastFixedPointUpdateCalculatesPIOutput)
{
    GPID pid(100.0f, 0.0f, 0.0f);
    pid.setGains(2.0f, 1.0f, 0.0f);

    gq15_16_t out = pid.update_pi_fast_q15_16(gq15_16_from_int(0), gq15_16_from_int(10));

    ASSERT_EQ(gq15_16_to_int(out), 30);
    ASSERT_FALSE(pid.saturated());
}

TEST(GpidTest, FastFixedPointUpdateSetsSaturationFlag)
{
    GPID pid(100.0f, 0.0f, 0.0f);
    pid.setGains(20.0f, 0.0f, 0.0f);

    ASSERT_EQ(gq15_16_to_int(pid.update_pi_fast_q15_16(
        gq15_16_from_int(0), gq15_16_from_int(10))), 100);
    ASSERT_TRUE(pid.saturated());

    ASSERT_EQ(gq15_16_to_int(pid.update_pi_fast_q15_16(
        gq15_16_from_int(10), gq15_16_from_int(0))), -100);
    ASSERT_TRUE(pid.saturated());
}

TEST(GpidTest, ResetClearsSaturationFlag)
{
    GPID pid(100.0f, 0.0f, 0.0f);
    pid.setGains(20.0f, 0.0f, 0.0f);
    pid.update_pi_fast_q15_16(gq15_16_from_int(0), gq15_16_from_int(10));
    ASSERT_TRUE(pid.saturated());

    pid.reset();

    ASSERT_FALSE(pid.saturated());
}

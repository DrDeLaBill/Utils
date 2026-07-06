/* Copyright © 2026 Georgy E. All rights reserved. */

#include <gtest/gtest.h>

#include <cmath>

#include "gpid.hpp"


TEST(GpidTest, OutputIsClampedByLimits)
{
    GPID pid(100.0f, 10.0f, 0.0f);
    pid.setGains(1000.0f, 0.0f, 0.0f);

    float out = pid.update(0.0f, 100.0f);

    ASSERT_TRUE(std::isfinite(out));
    ASSERT_LE(std::abs(out), 100.0f);
}


TEST(GpidTest, ResetAndDebugAreSafe)
{
    GPID pid(50.0f, 0.0f, 0.1f);
    pid.setGains(1.0f, 1.0f, 1.0f);

    (void)pid.update(1.0f, 2.0f);
    pid.reset();
    pid.setDebugEnabled(true, 1);
    pid.show();

    float out = pid.update(2.0f, 2.0f);
    ASSERT_TRUE(std::isfinite(out));
}

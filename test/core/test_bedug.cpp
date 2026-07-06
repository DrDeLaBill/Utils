/* Copyright © 2026 Georgy E. All rights reserved. */

#include <gtest/gtest.h>

#include "bedug.h"


TEST(BedugTest, AssertCallIsSafe)
{
    // This should never crash regardless of DEBUG configuration.
    b_assert("test_file.cpp", 42, "unit-test-check");
    SUCCEED();
}

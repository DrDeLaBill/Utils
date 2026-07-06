/* Copyright © 2026 Georgy E. All rights reserved. */

#include <gtest/gtest.h>

#include "CodeStopwatch.hpp"


TEST(CodeStopwatchTest, ConstructAndDestroyShortTag)
{
    utl::CodeStopwatch sw("UT", 0);
    SUCCEED();
}


TEST(CodeStopwatchTest, ConstructAndDestroyLongTag)
{
    utl::CodeStopwatch sw("THIS_TAG_IS_LONGER_THAN_BUFFER", 1);
    SUCCEED();
}

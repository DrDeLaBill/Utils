/* Copyright © 2025 Georgy E. All rights reserved. */

#include <iostream>
#include <cassert>
#include <gtest/gtest.h>

#include "CodeStopwatch.hpp"


static const char TAG[] = "TEST";


int main(int args, char** argv)
{
	utl::CodeStopwatch stopwatch(TAG);

    testing::InitGoogleTest(&args, argv);

	return RUN_ALL_TESTS();
}
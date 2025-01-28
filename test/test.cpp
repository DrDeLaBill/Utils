#include <iostream>
#include <cassert>
#include <gtest/gtest.h>

#include "CodeStopwatch.h"


static const char TAG[] = "TEST";


int main(int args, char** argv)
{
	utl::CodeStopwatch stopwatch(TAG);

    testing::InitGoogleTest(&args, argv);

	return RUN_ALL_TESTS();
}
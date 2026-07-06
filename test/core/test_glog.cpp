/* Copyright © 2026 Georgy E. All rights reserved. */

#include <gtest/gtest.h>

#include "glog.h"


TEST(GlogTest, MessageFilterBlocksImmediateRepeat)
{
    const uint32_t delay_ms = 5000;
    const char same_msg[] = "same-message";

    ASSERT_TRUE(__g_print_msg_filter_check(same_msg, delay_ms));
    ASSERT_FALSE(__g_print_msg_filter_check(same_msg, delay_ms));
}


TEST(GlogTest, MessageFilterIsIndependentPerMessage)
{
    const uint32_t delay_ms = 5000;
    const char msg_a[] = "message-a";
    const char msg_b[] = "message-b";

    ASSERT_TRUE(__g_print_msg_filter_check(msg_a, delay_ms));
    ASSERT_TRUE(__g_print_msg_filter_check(msg_b, delay_ms));
}

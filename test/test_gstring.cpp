/* Copyright © 2026 Georgy E. All rights reserved. */

#include <gtest/gtest.h>

#include <cstring>

#include "gstring.h"


TEST(UtilsFixture, util_add_char_left_alignment)
{
    char buf[16] = "abc";

    util_add_char(buf, sizeof(buf), '_', 7, ALIGN_MODE_LEFT);

    ASSERT_TRUE(strcmp(buf, "abc____") == 0);
}

TEST(UtilsFixture, util_add_char_right_alignment)
{
    char buf[16] = "abc";

    util_add_char(buf, sizeof(buf), '_', 7, ALIGN_MODE_RIGHT);

    ASSERT_TRUE(strcmp(buf, "____abc") == 0);
}

TEST(UtilsFixture, util_add_char_center_alignment)
{
    char buf[16] = "abc";

    util_add_char(buf, sizeof(buf), '_', 7, ALIGN_MODE_CENTER);

    ASSERT_TRUE(strcmp(buf, "__abc__") == 0);
}

TEST(UtilsFixture, util_add_char_respects_max_len)
{
    char buf[8] = "abc";

    util_add_char(buf, sizeof(buf), '_', 100, ALIGN_MODE_LEFT);

    ASSERT_TRUE(strlen(buf) == sizeof(buf) - 1);
    ASSERT_TRUE(strcmp(buf, "abc____") == 0);
}

TEST(UtilsFixture, util_add_char_invalid_mode_fallback_center)
{
    char buf[16] = "abc";

    util_add_char(buf, sizeof(buf), '_', 7, (ALIGN_MODE)99);

    ASSERT_TRUE(strcmp(buf, "__abc__") == 0);
}

TEST(UtilsFixture, util_int_to_str_with_point_positive)
{
    char out[32] = {0};

    util_int_to_str_with_point(out, sizeof(out), 12345, 100, 2, '.');

    ASSERT_TRUE(strcmp(out, "123.45") == 0);
}

TEST(UtilsFixture, util_int_to_str_with_point_negative)
{
    char out[32] = {0};

    util_int_to_str_with_point(out, sizeof(out), -12345, 100, 2, '.');

    ASSERT_TRUE(strcmp(out, "-123.45") == 0);
}

TEST(UtilsFixture, util_int_to_str_with_point_zero_fractional_digits)
{
    char out[32] = {0};

    util_int_to_str_with_point(out, sizeof(out), 12345, 100, 0, '.');

    ASSERT_TRUE(strcmp(out, "123") == 0);
}

TEST(UtilsFixture, util_int_to_str_with_point_custom_separator)
{
    char out[32] = {0};

    util_int_to_str_with_point(out, sizeof(out), 12345, 100, 2, ',');

    ASSERT_TRUE(strcmp(out, "123,45") == 0);
}

TEST(UtilsFixture, util_int_to_str_with_point_non_decimal_divider_fallback)
{
    char out[32] = {0};

    util_int_to_str_with_point(out, sizeof(out), 12345, 3, 2, '.');

    ASSERT_TRUE(strcmp(out, "4115.0") == 0);
}

TEST(UtilsFixture, util_u64_to_str_zero)
{
    char* str = util_u64_to_str(0ULL);

    ASSERT_TRUE(str != nullptr);
    ASSERT_TRUE(strcmp(str, "0") == 0);
}

TEST(UtilsFixture, util_u64_to_str_max_value)
{
    char* str = util_u64_to_str(18446744073709551615ULL);

    ASSERT_TRUE(str != nullptr);
    ASSERT_TRUE(strcmp(str, "18446744073709551615") == 0);
}

TEST(UtilsFixture, util_u64_to_str_reuse_static_buffer)
{
    char* first = util_u64_to_str(42ULL);
    char snapshot[32] = {0};
    strncpy(snapshot, first, sizeof(snapshot) - 1);

    char* second = util_u64_to_str(100ULL);

    ASSERT_TRUE(strcmp(snapshot, "42") == 0);
    ASSERT_TRUE(strcmp(second, "100") == 0);
}

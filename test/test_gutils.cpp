/* Copyright © 2026 Georgy E. All rights reserved. */

#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "gutils.h"


TEST(UtilsFixture, util_get_number_len_for_positive_number)
{
    ASSERT_TRUE(util_get_number_len(12345) == 5);
}

TEST(UtilsFixture, util_get_number_len_for_zero)
{
    ASSERT_TRUE(util_get_number_len(0) == 0);
}

TEST(UtilsFixture, util_get_number_len_for_negative_number)
{
    ASSERT_TRUE(util_get_number_len(-123) == 3);
}

TEST(UtilsFixture, util_small_pow_regular_case)
{
    ASSERT_TRUE(util_small_pow(2, 10) == 1024);
}

TEST(UtilsFixture, util_small_pow_zero_degree)
{
    ASSERT_TRUE(util_small_pow(7, 0) == 1);
}

TEST(UtilsFixture, util_convert_range_linear_mapping)
{
    int mapped = util_convert_range(50, 0, 100, 0, 200);

    ASSERT_TRUE(mapped == 100);
}

TEST(UtilsFixture, util_hash_is_deterministic)
{
    const uint8_t data[] = {'a', 'b', 'c', 'd'};

    unsigned h1 = util_hash(data, sizeof(data));
    unsigned h2 = util_hash(data, sizeof(data));

    ASSERT_TRUE(h1 == h2);
}

TEST(UtilsFixture, util_hash_changes_for_different_input)
{
    const uint8_t a[] = {'a', 'b', 'c'};
    const uint8_t b[] = {'a', 'b', 'd'};

    unsigned ha = util_hash(a, sizeof(a));
    unsigned hb = util_hash(b, sizeof(b));

    ASSERT_TRUE(ha != hb);
}

TEST(UtilsFixture, util_memfind_finds_pattern_in_middle)
{
    uint8_t buf[] = {1, 2, 3, 4, 5, 6};
    const uint8_t pattern[] = {3, 4, 5};

    uint8_t* pos = util_memfind(buf, sizeof(buf), pattern, sizeof(pattern));

    ASSERT_TRUE(pos != nullptr);
    ASSERT_TRUE((size_t)(pos - buf) == 2);
}

TEST(UtilsFixture, util_memfind_finds_pattern_at_start)
{
    uint8_t buf[] = {9, 8, 7, 6};
    const uint8_t pattern[] = {9, 8};

    uint8_t* pos = util_memfind(buf, sizeof(buf), pattern, sizeof(pattern));

    ASSERT_TRUE(pos == &buf[0]);
}

TEST(UtilsFixture, util_memfind_returns_null_when_not_found)
{
    uint8_t buf[] = {1, 2, 3, 4};
    const uint8_t pattern[] = {5, 6};

    uint8_t* pos = util_memfind(buf, sizeof(buf), pattern, sizeof(pattern));

    ASSERT_TRUE(pos == nullptr);
}

TEST(UtilsFixture, util_memfind_returns_null_for_zero_sizes)
{
    uint8_t buf[] = {1, 2, 3};
    const uint8_t pattern[] = {1};

    ASSERT_TRUE(util_memfind(buf, 0, pattern, sizeof(pattern)) == nullptr);
    ASSERT_TRUE(util_memfind(buf, sizeof(buf), pattern, 0) == nullptr);
}

static bool always_true_condition(void)
{
    return true;
}

static bool always_false_condition(void)
{
    return false;
}

TEST(UtilsFixture, util_wait_event_returns_true_when_condition_is_immediately_true)
{
    bool ok = util_wait_event(always_true_condition, 20);

    ASSERT_TRUE(ok);
}

TEST(UtilsFixture, util_wait_event_returns_false_when_condition_never_true)
{
    bool ok = util_wait_event(always_false_condition, 1);

    ASSERT_TRUE(!ok);
}

TEST(UtilsFixture, util_debug_hex_dump_no_crash_for_non_empty_buffer)
{
    const uint8_t buf[] = {0x10, 0x20, 0x30, 0x40};

    util_debug_hex_dump(buf, 0, (uint16_t)sizeof(buf));

    SUCCEED();
}

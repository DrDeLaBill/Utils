/* Copyright © 2025 Georgy E. All rights reserved. */

#include <gtest/gtest.h>

#include "gsort.hpp"
#include "CircleBuffer.hpp"

// Basic smoke tests: empty and single-element
TEST(GSortTest, HandlesEmptyAndSingle)
{
    // empty: should not crash
    gsort<int>(nullptr, 0);

    int single[] = {42};
    gsort(single, 1);
    EXPECT_EQ(42, single[0]);
}

TEST(GSortTest, SortsIntegers)
{
    int data[] = {3, 1, 4, 1, 5, 9, 2, 6};
    const int expected[] = {1,1,2,3,4,5,6,9};
    gsort(data, sizeof(data)/sizeof(data[0]));
    for (size_t i = 0; i < sizeof(data)/sizeof(data[0]); ++i) {
        EXPECT_EQ(expected[i], data[i]);
    }
}

TEST(GSortTest, SortsWithDuplicates)
{
    int data[] = {2,2,1,2,1,1};
    const int expected[] = {1,1,1,2,2,2};
    gsort(data, 6);
    for (size_t i = 0; i < 6; ++i) EXPECT_EQ(expected[i], data[i]);
}

TEST(GSortTest, SortsFloatsAndNegatives)
{
    float data[] = {3.5f, -1.0f, 2.0f, -1.0f, 0.0f};
    const float expected[] = {-1.0f, -1.0f, 0.0f, 2.0f, 3.5f};
    gsort(data, 5);
    for (size_t i = 0; i < 5; ++i) EXPECT_FLOAT_EQ(expected[i], data[i]);
}

TEST(GSortTest, WorksWithUnsignedTypes)
{
    uint8_t data[] = {200, 10, 255, 0, 128};
    const uint8_t expected[] = {0,10,128,200,255};
    gsort(data, 5);
    for (size_t i = 0; i < 5; ++i) EXPECT_EQ(expected[i], data[i]);
}

// Tests for gmedian
TEST(GSortTest, MedianOddSize)
{
    int data[] = {5, 1, 3};
    int med = gmedian(data, 3);
    EXPECT_EQ(3, med);
}

TEST(GSortTest, MedianEvenSizeReturnsUpperMiddle)
{
    // Implementation currently returns tmpBuf[size/2], i.e. upper middle for even sizes
    int data[] = {4, 1, 2, 3};
    int med = gmedian(data, 4);
    // sorted = {1,2,3,4}, index 4/2 == 2 -> value 3
    EXPECT_EQ(3, med);
}

TEST(GSortTest, MedianDoesNotModifySource)
{
    int src[] = {9, 7, 5, 3, 1};
    int copy[] = {9, 7, 5, 3, 1};
    int med = gmedian(src, 5);
    EXPECT_EQ(5, med);
    // ensure original unchanged
    for (size_t i = 0; i < 5; ++i) EXPECT_EQ(copy[i], src[i]);
}

// Tests that use utl::CircleBuffer directly
TEST(GSortTest, SortsIntoCircleBufferUsingGet)
{
    // Use a power-of-two size greater than number of elements to avoid wrap
    utl::CircleBuffer<8, int> buf;
    buf.clear();
    buf.push_back(7);
    buf.push_back(3);
    buf.push_back(5);
    buf.push_back(1);

    // When readCount == 0 and no wrap occurred, get() points to logical sequence
    gsort<int>(const_cast<int*>(buf.get()), buf.count());

    const int expected[] = {1,3,5,7};
    for (size_t i = 0; i < buf.count(); ++i) {
        EXPECT_EQ(expected[i], buf[i]);
    }
}

TEST(GSortTest, MedianFromCircleBufferLogicalOrder)
{
    utl::CircleBuffer<8, int> buf;
    buf.clear();
    buf.push_back(9);
    buf.push_back(2);
    buf.push_back(6);
    buf.push_back(4);
    buf.push_back(8);

    // Extract logical contents into a temporary array and compute median
    const size_t n = buf.count();
    int tmp[8] = {};
    for (size_t i = 0; i < n; ++i) tmp[i] = buf[i];

    int med = gmedian(tmp, n);
    // sorted = {2,4,6,8,9} -> median = 6
    EXPECT_EQ(6, med);
    // Original buffer must remain unchanged
    EXPECT_EQ(9, buf[0]);
}

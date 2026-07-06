/* Copyright © 2025 Georgy E. All rights reserved. */

#include <gtest/gtest.h>

#include "CircleBuffer.hpp"


using namespace utl;


GTEST_TEST(CircleBufferSuite, TestInitialState) {
    CircleBuffer<8> buffer;
    ASSERT_TRUE(buffer.empty());
    ASSERT_EQ(buffer.count(), 0);
}

GTEST_TEST(CircleBufferSuite, TestPushBackIncreasesCount) {
    CircleBuffer<4, int> buffer;
    buffer.push_back(1);
    ASSERT_EQ(buffer.count(), 1);
    buffer.push_back(2);
    ASSERT_EQ(buffer.count(), 2);
}

GTEST_TEST(CircleBufferSuite, TestPushBackWhenFullRemovesOldest) {
    CircleBuffer<2, int> buffer;
    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);
    ASSERT_EQ(buffer.count(), 2);
    ASSERT_EQ(buffer[0], 2);
    ASSERT_EQ(buffer[1], 3);
}

GTEST_TEST(CircleBufferSuite, TestPushFrontAddsToFront) {
    CircleBuffer<2, int> buffer;
    buffer.push_front(1);
    buffer.push_front(2);
    buffer.push_front(3);
    ASSERT_EQ(buffer.count(), 2);
    ASSERT_EQ(buffer[0], 3);
    ASSERT_EQ(buffer[1], 2);
}

GTEST_TEST(CircleBufferSuite, TestPopFrontRemovesFirst) {
    CircleBuffer<4, int> buffer;
    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);
    auto front = buffer.pop_front();
    ASSERT_EQ(front, 1);
    ASSERT_EQ(buffer.count(), 2);
    ASSERT_EQ(buffer[0], 2);
}

GTEST_TEST(CircleBufferSuite, TestPopBackRemovesLast) {
    CircleBuffer<4, int> buffer;
    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);
    auto back = buffer.pop_back();
    ASSERT_EQ(back, 3);
    ASSERT_EQ(buffer.count(), 2);
    ASSERT_EQ(buffer[1], 2);
}

GTEST_TEST(CircleBufferSuite, TestFullWhenSizeReached) {
    CircleBuffer<2, int> buffer;
    buffer.push_back(1);
    buffer.push_back(2);
    ASSERT_TRUE(buffer.full());
}

GTEST_TEST(CircleBufferSuite, TestClearResetsBuffer) {
    CircleBuffer<4, int> buffer;
    buffer.push_back(1);
    buffer.clear();
    ASSERT_TRUE(buffer.empty());
    ASSERT_EQ(buffer.count(), 0);
}

GTEST_TEST(CircleBufferSuite, TestOperatorAccessCorrect) {
    CircleBuffer<4, int> buffer;
    buffer.push_back(10);
    buffer.push_back(20);
    buffer.push_back(30);
    ASSERT_EQ(buffer[0], 10);
    buffer.pop_front();
    ASSERT_EQ(buffer[0], 20);
}

GTEST_TEST(CircleBufferSuite, TestShiftRetrievesFrontElement) {
    CircleBuffer<4, int> buffer;
    buffer.push_back(5);
    int value;
    bool success = buffer.shift(value);
    ASSERT_TRUE(success);
    ASSERT_EQ(value, 5);
    ASSERT_TRUE(buffer.empty());
}

GTEST_TEST(CircleBufferSuite, TestFrontAndBackReturnCorrectElements) {
    CircleBuffer<4, int> buffer;
    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);
    ASSERT_EQ(buffer.front(), 1);
    ASSERT_EQ(buffer.back(), 3);
}

GTEST_TEST(CircleBufferSuite, TestPushFrontWhenFullPopsBack) {
    CircleBuffer<2, int> buffer;
    buffer.push_front(1);
    buffer.push_front(2);
    buffer.push_front(3);
    ASSERT_EQ(buffer.count(), 2);
    ASSERT_EQ(buffer[0], 3);
    ASSERT_EQ(buffer[1], 2);
}

GTEST_TEST(CircleBufferSuite, TestSizeMethodReturnsCorrectValue) {
    CircleBuffer<8, int> buffer;
    ASSERT_EQ(buffer.size(), 8);
}
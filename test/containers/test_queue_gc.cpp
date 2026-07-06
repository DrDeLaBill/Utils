/* Copyright © 2026 Georgy E. All rights reserved. */

#include <gtest/gtest.h>

#include "queue_gc.h"


TEST(QueueGcTest, InitPushPopFifo)
{
    queue_gc_t q;
    uint32_t storage[4] = {};

    ASSERT_TRUE(queue_gc_init(&q, (uint8_t*)storage, sizeof(storage[0]), 4));
    ASSERT_TRUE(queue_gc_empty(&q));

    uint32_t v1 = 11;
    uint32_t v2 = 22;
    uint32_t v3 = 33;

    queue_gc_push(&q, (const uint8_t*)&v1);
    queue_gc_push(&q, (const uint8_t*)&v2);
    queue_gc_push(&q, (const uint8_t*)&v3);

    ASSERT_EQ(queue_gc_count(&q), 3u);
    ASSERT_EQ(*(uint32_t*)queue_gc_peek(&q), v1);
    ASSERT_EQ(*(uint32_t*)queue_gc_back(&q), v3);

    ASSERT_EQ(*(uint32_t*)queue_gc_pop(&q), v1);
    ASSERT_EQ(*(uint32_t*)queue_gc_pop(&q), v2);
    ASSERT_EQ(*(uint32_t*)queue_gc_pop(&q), v3);
    ASSERT_TRUE(queue_gc_empty(&q));
}


TEST(QueueGcTest, FullQueueOverwritesOldest)
{
    queue_gc_t q;
    uint32_t storage[2] = {};

    ASSERT_TRUE(queue_gc_init(&q, (uint8_t*)storage, sizeof(storage[0]), 2));

    uint32_t a = 1;
    uint32_t b = 2;
    uint32_t c = 3;

    queue_gc_push(&q, (const uint8_t*)&a);
    queue_gc_push(&q, (const uint8_t*)&b);
    ASSERT_TRUE(queue_gc_full(&q));

    queue_gc_push(&q, (const uint8_t*)&c);

    ASSERT_EQ(queue_gc_count(&q), 2u);
    ASSERT_EQ(*(uint32_t*)queue_gc_pop(&q), b);
    ASSERT_EQ(*(uint32_t*)queue_gc_pop(&q), c);
    ASSERT_TRUE(queue_gc_empty(&q));
}

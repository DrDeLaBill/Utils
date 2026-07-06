/* Copyright © 2025 Georgy E. All rights reserved. */

#include "queue_gc.h"


bool queue_gc_init(queue_gc_t* p, uint8_t* ptr, unsigned unit_size, unsigned length)
{
    return circle_buf_gc_init(&p->buf, ptr, unit_size, length);
}

unsigned queue_gc_count(const queue_gc_t* p)
{
    return circle_buf_gc_count(&p->buf);
}

void queue_gc_free(queue_gc_t* p)
{
    circle_buf_gc_free(&p->buf);
}

bool queue_gc_empty(const queue_gc_t* p)
{
    return circle_buf_gc_empty(&p->buf);
}

bool queue_gc_full(const queue_gc_t* p)
{
    return circle_buf_gc_full(&p->buf);
}

void queue_gc_push(queue_gc_t* p, const uint8_t* data)
{
    circle_buf_gc_push_back(&p->buf, data);
}

uint8_t* queue_gc_pop(queue_gc_t* p)
{
    return circle_buf_gc_pop_front(&p->buf);
}

uint8_t* queue_gc_peek(queue_gc_t* p)
{
    return circle_buf_gc_front(&p->buf);
}

uint8_t* queue_gc_back(queue_gc_t* p)
{
    return circle_buf_gc_back(&p->buf);
}

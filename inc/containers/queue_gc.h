/* Copyright © 2025 Georgy E. All rights reserved. */

#ifndef _QUEUE_GC_H_
#define _QUEUE_GC_H_


#ifdef __cplusplus
extern "C" {
#endif


#include "circle_buf_gc.h"


typedef struct _queue_gc_t {
    circle_buf_gc_t buf;
} queue_gc_t;


bool queue_gc_init(queue_gc_t* p, uint8_t* ptr, unsigned unit_size, unsigned length);
unsigned queue_gc_count(const queue_gc_t* p);
void queue_gc_free(queue_gc_t* p);

bool queue_gc_empty(const queue_gc_t* p);
bool queue_gc_full(const queue_gc_t* p);

void queue_gc_push(queue_gc_t* p, const uint8_t* data);
uint8_t* queue_gc_pop(queue_gc_t* p);
uint8_t* queue_gc_peek(queue_gc_t* p);
uint8_t* queue_gc_back(queue_gc_t* p);


#ifdef __cplusplus
}
#endif


#endif

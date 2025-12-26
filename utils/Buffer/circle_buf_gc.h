/* Copyright © 2024 Georgy E. All rights reserved. */

#ifndef _CIRCLE_BUF_GC_H_
#define _CIRCLE_BUF_GC_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>

#include "gutils.h"


#define CIRCLE_BUFFER_GC_HEADER_INIT(SIZE, TYPE, CIRCLE_BUF_NAME) \
    TYPE __concat(__circle_buf_array_, CIRCLE_BUF_NAME)[SIZE]; \
    circle_buf_gc_t CIRCLE_BUF_NAME;

#define CIRCLE_BUFFER_GC_SRC_INIT(CIRCLE_BUF_NAME) \
    circle_buf_gc_init(&CIRCLE_BUF_NAME, (uint8_t*)&__concat(__circle_buf_array_, CIRCLE_BUF_NAME), sizeof(*__concat(__circle_buf_array_, CIRCLE_BUF_NAME)), __arr_len(__concat(__circle_buf_array_, CIRCLE_BUF_NAME)));


typedef struct _circle_buf_gc_t {
    uint32_t m_bedacode;
    uint8_t* m_data;
    unsigned m_unit_size;
    unsigned m_length;
    unsigned m_read_idx;
    unsigned m_write_cnt;
} circle_buf_gc_t;


bool circle_buf_gc_init(circle_buf_gc_t* p, uint8_t* ptr, unsigned unit_size, unsigned length);
unsigned circle_buf_gc_count(const circle_buf_gc_t* p);
void circle_buf_gc_free(circle_buf_gc_t* p);

bool circle_buf_gc_empty(const circle_buf_gc_t* p);
bool circle_buf_gc_full(const circle_buf_gc_t* p);

void circle_buf_gc_push_back(circle_buf_gc_t* p, const uint8_t* data);
void circle_buf_gc_push_front(circle_buf_gc_t* p, const uint8_t* data);
uint8_t* circle_buf_gc_pop_front(circle_buf_gc_t* p);
uint8_t* circle_buf_gc_pop_back(circle_buf_gc_t* p);
uint8_t* circle_buf_gc_front(circle_buf_gc_t* p);
uint8_t* circle_buf_gc_back(circle_buf_gc_t* p);
uint8_t* circle_buf_gc_index(circle_buf_gc_t* p, const unsigned index);


#ifdef __cplusplus
}
#endif


#endif

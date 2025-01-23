/* Copyright © 2024 Georgy E. All rights reserved. */

#ifndef _CIRCLE_BUF_GC_H_
#define _CIRCLE_BUF_GC_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>


typedef struct _circle_buf_gc_t {
	uint32_t m_bedacode;
    uint8_t* m_data;
    unsigned m_unit_size;
    unsigned m_length;
    unsigned m_read_idx;
    unsigned m_write_cnt;
} circle_buf_gc_t;


void circle_buf_gc_init(circle_buf_gc_t* p, uint8_t* ptr, unsigned unit_size, unsigned length);
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


#ifdef __cplusplus
}
#endif


#endif

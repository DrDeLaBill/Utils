/* Copyright © 2024 Georgy E. All rights reserved. */

#include "circle_buf_gc.h"

#include <string.h>

#include "bmacro.h"

#define BEDACODE (0xBEDAC2DE)


static unsigned _ptr_index_from_read(const circle_buf_gc_t* p, unsigned index);


void circle_buf_gc_init(circle_buf_gc_t* p, uint8_t* ptr, unsigned unit_size, unsigned length)
{
    if (!p) {
        BEDUG_ASSERT(false, "bad buffer");
        return;
    }
    memset((uint8_t*)p, 0, sizeof(circle_buf_gc_t));
    if (!ptr) {
        BEDUG_ASSERT(false, "bad buffer");
        return;
    }
    p->m_bedacode  = BEDACODE;
    p->m_data      = ptr;
    p->m_unit_size = unit_size;
    p->m_length    = length;
}

unsigned circle_buf_gc_count(const circle_buf_gc_t* p) 
{
    if (!p || p->m_bedacode != BEDACODE) {
        BEDUG_ASSERT(false, "uninitialized buffer");
        return 0;
    }
    return p->m_write_cnt;
}

void circle_buf_gc_free(circle_buf_gc_t* p)
{
    if (!p || p->m_bedacode != BEDACODE) {
        BEDUG_ASSERT(false, "uninitialized buffer");
        return;
    }
    p->m_read_idx = 0;
    p->m_write_cnt = 0;
}

bool circle_buf_gc_empty(const circle_buf_gc_t* p) 
{
	if (p && p->m_bedacode == BEDACODE) {
		return circle_buf_gc_count(p) == 0;
	}
    BEDUG_ASSERT(false, "uninitialized buffer");
    return false;
}

bool circle_buf_gc_full(const circle_buf_gc_t* p) 
{
	if (p && p->m_bedacode == BEDACODE) {
		return circle_buf_gc_count(p) == p->m_length;
	}
    BEDUG_ASSERT(false, "uninitialized buffer");
    return false;
}

void circle_buf_gc_push_back(circle_buf_gc_t* p, const uint8_t* data) 
{
    if (!p || !data || p->m_bedacode != BEDACODE) {
        BEDUG_ASSERT(false, "uninitialized buffer");
        return;
    }
    if (circle_buf_gc_full(p)) {
        circle_buf_gc_pop_front(p);
    }
    memcpy(p->m_data + _ptr_index_from_read(p, p->m_write_cnt++), data, p->m_unit_size);
}

void circle_buf_gc_push_front(circle_buf_gc_t* p, const uint8_t* data) 
{
    if (!p || !data || p->m_bedacode != BEDACODE) {
        BEDUG_ASSERT(false, "uninitialized buffer");
        return;
    }
    if (circle_buf_gc_full(p)) {
        circle_buf_gc_pop_back(p);
    }
    p->m_read_idx = (p->m_read_idx == 0) ? p->m_length - 1 : p->m_read_idx - 1;
    memcpy(p->m_data + _ptr_index_from_read(p, 0), data, p->m_unit_size);
    p->m_write_cnt++;
}

uint8_t* circle_buf_gc_pop_front(circle_buf_gc_t* p) 
{
    if (circle_buf_gc_empty(p)) {
        BEDUG_ASSERT(false, "error pop front unit - empty");
        return NULL;
    }
    uint8_t* ptr = p->m_data + _ptr_index_from_read(p, 0);
    p->m_read_idx = (p->m_read_idx + 1) % p->m_length;
    p->m_write_cnt--;
    return ptr;
}

uint8_t* circle_buf_gc_pop_back(circle_buf_gc_t* p) 
{
    if (circle_buf_gc_empty(p)) {
        BEDUG_ASSERT(false, "error pop back unit - empty");
        return NULL;
    }
    return p->m_data + _ptr_index_from_read(p, --p->m_write_cnt);
}

uint8_t* circle_buf_gc_front(circle_buf_gc_t* p) 
{
    if (circle_buf_gc_empty(p)) {
        BEDUG_ASSERT(false, "error front unit - empty");
        return NULL;
    }
    return p->m_data + _ptr_index_from_read(p, 0);
}

uint8_t* circle_buf_gc_back(circle_buf_gc_t* p) 
{
    if (circle_buf_gc_empty(p)) {
        BEDUG_ASSERT(false, "error back unit - empty");
        return NULL;
    }
    return p->m_data + _ptr_index_from_read(p, p->m_write_cnt - 1);
}

unsigned _ptr_index_from_read(const circle_buf_gc_t* p, unsigned index) 
{
    return ((p->m_read_idx + index) % p->m_length) * p->m_unit_size;
}

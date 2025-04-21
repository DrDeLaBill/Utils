/* Copyright © 2023 Georgy E. All rights reserved. */

#include "gutils.h"

#include <stdint.h>
#include <stdbool.h>

#include "glog.h"


void gtimer_start(gtimer_t* timer, TIME_MS_T delay) {
    timer->start = getMillis();
    timer->delay = delay;
}

bool gtimer_wait(gtimer_t* tm) {
    return ((TIME_MS_T)((TIME_MS_T)getMillis() - (TIME_MS_T)tm->start)) < ((TIME_MS_T)tm->delay);
}

void gtimer_reset(gtimer_t* tm)
{
	tm->delay = 0;
	tm->start = 0;
}

uint32_t gtimer_remaining(gtimer_t* tm)
{
    if (tm->start + tm->delay < getMillis()) {
    	return 0;
    }
    if (tm->start > getMillis()) {
    	return 0;
    }
    return tm->delay - (getMillis() - tm->start);
}

#if defined(_DEBUG) || defined(DEBUG)
void util_debug_hex_dump(const uint8_t* buf, uint32_t start_counter, uint16_t len) {
    const uint8_t cols_count = 16;
    uint32_t i = 0;
    printPretty("- offset -    0  1  2  3  4  5  6  7 |  8  9  A  B  C  D  E  F | 0123456789ABCDEF\n");
    do {
        printPretty("0x%08X:  ", (unsigned int)(start_counter + i * cols_count));
        for (uint32_t j = 0; j < cols_count; j++) {
            if (i * cols_count + j > len) {
            	gprint("   ");
            } else {
            	gprint("%02X ", buf[i * cols_count + j]);
            }
            if ((j + 1) % 8 == 0) {
            	gprint("| ");
            }
        }
        for (uint32_t j = 0; j < cols_count; j++) {
            if (i * cols_count + j > len) {
                break;
            }
            char c = buf[i * cols_count + j];
            if (c > 31 && c < 0xFF) {
                gprint("%c", (char)c);
            } else {
            	gprint(".");
            }
        }
        gprint("\n");
        i++;
    } while (i * cols_count < len);
}
#else
void util_debug_hex_dump(const uint8_t* buf, uint32_t start_counter, uint16_t len) 
{
    (void)buf;
    (void)start_counter;
    (void)len;
}
#endif

bool util_wait_event(bool (*condition) (void), TIME_MS_T time)
{
    TIME_MS_T start_time = getMillis();
    while (__abs_dif(start_time, getMillis()) < time) {
        if (condition()) {
            return true;
        }
    }
    return false;
}

uint8_t util_get_number_len(int number)
{
    uint8_t counter = 0;
    while (number) {
        number /= 10;
        counter++;
    }
    return counter;
}

uint32_t util_small_pow(const uint32_t number, uint32_t degree)
{
	if (!degree) {
		return 1;
	}
	uint32_t res = number;
	while (--degree) {
		res *= number;
	}
	return res;
}

int util_convert_range(int val, int rngl1, int rngh1, int rngl2, int rngh2)
{
	int range1 = __abs_dif(rngh1, rngl1);
	int range2 = __abs_dif(rngh2, rngl2);
	int delta  = __abs_dif(rngl1, val);
    return rngl2 + ((delta * range2) / range1);
}

unsigned util_hash(const uint8_t* data, const unsigned size)
{
	unsigned hash = 0;

	for (unsigned i = 0; i < size; i++) {
		uint8_t symbol = data[i];
        for (unsigned j = sizeof (char) * 8; j > 0; j--) {
        	hash = ((hash ^ (unsigned)symbol) & 1) ? (hash >> 1) ^ 0x8C : (hash >> 1);
            symbol >>= 1;
        }
        hash <<= 1;
	}

	return hash;
}

uint8_t* util_memfind(uint8_t* buf, const size_t buf_size, const uint8_t* pattern, const size_t pattern_size)
{
    if (!buf_size || !pattern_size) {
        return NULL;
    }
    for (size_t i = 0; i < buf_size; i++) {
        if (buf_size - i < pattern_size) {
            return NULL;
        }
        if (!memcmp(buf + i, pattern, pattern_size)) {
            return &buf[i];
        }
    }
    return NULL;
}


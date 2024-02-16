/* Copyright © 2023 Georgy E. All rights reserved. */

#include <log.h>
#include "utils.h"

#include <stdint.h>
#include <stdbool.h>

#include "gtime.h"
#ifdef USE_HAL_DRIVER
#   include "hal_defs.h"
#endif


void util_old_timer_start(util_old_timer_t* timer, uint32_t delay) {
    timer->start = getMillis();
    timer->delay = delay;
}

bool util_old_timer_wait(util_old_timer_t* tm) {
    return ((uint32_t)((uint32_t)getMillis() - (uint32_t)tm->start)) < ((uint32_t)tm->delay);
}


#if defined(_DEBUG) || defined(DEBUG) || !defined(QT_NO_DEBUG)
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
    (void*)buf;
    (void)start_counter;
    (void)len;
}
#endif

bool util_wait_event(bool (*condition) (void), uint32_t time)
{
    uint32_t start_time = getMillis();
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
	uint32_t res = number;
	while (--degree) {
		res *= number;
	}
	return res;
}

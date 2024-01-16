/* Copyright © 2023 Georgy E. All rights reserved. */

#include "utils.h"

#include <stdint.h>
#include <stdbool.h>

#include "log.h"
#include "gtime.h"
#include "hal_defs.h"


#if defined(_DEBUG) || defined(DEBUG)
void util_debug_hex_dump(const uint8_t* buf, uint32_t start_counter, uint16_t len) {
    const uint8_t cols_count = 16;
    uint32_t i = 0;
    printPretty("- offset -    0  1  2  3  4  5  6  7 |  8  9  A  B  C  D  E  F | 0123456789ABCDEF\n");
    do {
        printPretty("0x%08X:  ", (unsigned int)(start_counter + i * cols_count));
        for (uint32_t j = 0; j < cols_count; j++) {
            if (i * cols_count + j > len) {
            	print("   ");
            } else {
            	print("%02X ", buf[i * cols_count + j]);
            }
            if ((j + 1) % 8 == 0) {
            	print("| ");
            }
        }
        for (uint32_t j = 0; j < cols_count; j++) {
            if (i * cols_count + j > len) {
                break;
            }
            char c = buf[i * cols_count + j];
            if (c > 31 && c != 0xFF) {
                print("%c", (char)c);
            } else {
            	print(".");
            }
        }
        print("\n");
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

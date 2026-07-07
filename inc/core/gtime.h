/* Copyright © 2026 Georgy E. All rights reserved. */

#ifndef __G_TIME_H
#define __G_TIME_H


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>


typedef struct _gtimer_t {
    uint32_t start;
    uint32_t delay;
} gtimer_t;


void gtimer_start(gtimer_t* tm, uint32_t waitMs);
void gtimer_reload(gtimer_t* tm);
bool gtimer_wait(gtimer_t* tm);
uint32_t gtimer_remaining(gtimer_t* tm);
void gtimer_reset(gtimer_t* tm);


uint32_t getMillis(void);
uint64_t getMillis64bit(void);
uint64_t getMicroseconds(void);


#ifdef __cplusplus
}
#endif


#endif

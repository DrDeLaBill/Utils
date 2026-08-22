/* Copyright © 2026 Georgy E. All rights reserved. */

#ifndef _FILTER_EMA_H_
#define _FILTER_EMA_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>


typedef struct _filter_ema_t {
    uint8_t K;
    int32_t acc;
    volatile int32_t val;
} filter_ema_t;


bool filter_ema_init(filter_ema_t* filter, uint8_t shift);
void filter_ema_update(filter_ema_t* filter, int32_t new_value);
int32_t filter_ema_get(const filter_ema_t* filter);


#ifdef __cplusplus
}
#endif


#endif
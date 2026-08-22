/* Copyright © 2026 Georgy E. All rights reserved. */

#include "filters.h"

#include <limits.h>


bool filter_ema_init(filter_ema_t* filter, uint8_t shift)
{
    if (!filter || shift >= 31) {
        return false;
    }

    filter->K = shift;
    filter->acc = 0;
    filter->val = 0;
    return true;
}

void filter_ema_update(filter_ema_t* filter, int32_t new_value)
{
    if (!filter || filter->K >= 31) {
        return;
    }

    int64_t next_acc = (int64_t)filter->acc - (filter->acc >> filter->K) + new_value;
    if (next_acc > INT32_MAX) {
        next_acc = INT32_MAX;
    } else if (next_acc < INT32_MIN) {
        next_acc = INT32_MIN;
    }

    filter->acc = (int32_t)next_acc;
    filter->val = filter->acc >> filter->K;
}

int32_t filter_ema_get(const filter_ema_t* filter)
{
    if (!filter || filter->K >= 31) {
        return 0;
    }
    return filter->val;
}
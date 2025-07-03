/* Copyright © 2025 Georgy E. All rights reserved. */

#include "glog.h"

#include <stddef.h>
#include <string.h>

#include "gutils.h"
#include "bmacro.h"
#include "circle_buf_gc.h"


#ifdef GPRINT_ENABLED


#define __GLOG_MILLIS_SYMBOLS_CNT_ 8
#define __GLOG_MILLIS_TIME_DIV_    100000000
#define __GLOG_TOSTRING_(x)        __STR_DEF__(x)
#define __GLOG_PRETTY_LOG_OFFSET_  (__GLOG_MILLIS_SYMBOLS_CNT_ + 8)


void __g_print_tag(const char* tag)
{
    const int GLOG_TAG_MAX_SIZE = 5;
    size_t offset = strlen(tag) > GLOG_TAG_MAX_SIZE + 1 ? 1 : GLOG_TAG_MAX_SIZE - (int)strlen(tag);
    printMessage("%0" __GLOG_TOSTRING_(__GLOG_MILLIS_SYMBOLS_CNT_) __G_TIME_PRINT_LEN "->", getMillis() % __GLOG_MILLIS_TIME_DIV_);
    printMessage("%s:%*s", tag, offset, "");
}

void __g_print_offset()
{
    printMessage("%*s", __GLOG_PRETTY_LOG_OFFSET_, "");
}


#define __GPRINT_MSG_FILTER_CNT (10)

typedef struct __g_print_msg_filter_t {
    size_t*  ptr;
    gtimer_t timer;
} g_print_msg_filter_t;

g_print_msg_filter_t g_print_msg_filters[__GPRINT_MSG_FILTER_CNT] = {0};
circle_buf_gc_t g_print_msg_filters_buf = {0};

bool  __g_print_msg_filter_check(const char* msg, TIME_MS_T delay_ms)
{
    static bool initialized = false;
    if (!initialized) {
        circle_buf_gc_init(&g_print_msg_filters_buf, (uint8_t*)&g_print_msg_filters, sizeof(g_print_msg_filter_t), __arr_len(g_print_msg_filters));
        initialized = true;
    }
    size_t* msg_hash = (size_t*)msg;
    for (unsigned i = 0; i < circle_buf_gc_count(&g_print_msg_filters_buf); i++) {
        g_print_msg_filter_t* ptr = ((g_print_msg_filter_t*)circle_buf_gc_index(&g_print_msg_filters_buf, i));
        if (ptr->ptr != msg_hash) {
            continue;
        }
        if (!gtimer_wait(&ptr->timer)) {
            gtimer_start(&ptr->timer, delay_ms);
            return true;
        }
        return false;
    }
    if (circle_buf_gc_full(&g_print_msg_filters_buf)) {
        printTagLog("INFO", "Message filter buffer is full");
        circle_buf_gc_pop_front(&g_print_msg_filters_buf);
    }
    g_print_msg_filter_t filter = {0};
    gtimer_start(&filter.timer, delay_ms);
    filter.ptr = msg_hash;
    circle_buf_gc_push_back(&g_print_msg_filters_buf, (uint8_t*)&filter);
    return true;
}

#endif
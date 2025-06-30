/* Copyright © 2025 Georgy E. All rights reserved. */

#include "glog.h"

#include <stddef.h>
#include <string.h>

#include "bmacro.h"


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
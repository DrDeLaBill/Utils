/* Copyright © 2025 Georgy E. All rights reserved. */

#include "glob.h"

#include <stddef.h>
#include <string.h>


#define GLOG_TAG_MAX_SIZE (5)


int __gprint_tag_offset(const char* tag) 
{
    return strlen(tag) > GLOG_TAG_MAX_SIZE + 1 ? 1 : GLOG_TAG_MAX_SIZE - strlen(tag);
}
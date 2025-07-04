/* Copyright © 2024 Georgy E. All rights reserved. */

#ifndef _G_STRING_H_
#define _G_STRING_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <stddef.h>
#include <stdint.h>

#include "gutils.h"


void util_add_char(char* phrase, size_t max_len, char symbol, size_t target_len, ALIGN_MODE mode);
void util_int_to_str_with_point(char* target, unsigned size, int value, unsigned div, unsigned point_count, char point);
char* util_u64_to_str(uint64_t value);


#ifdef __cplusplus
}
#endif


#endif

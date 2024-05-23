/* Copyright © 2024 Georgy E. All rights reserved. */

#ifndef _G_STRING_H_
#define _G_STRING_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <stddef.h>
#include <stdint.h>

#include "utils.h"


void util_add_char(char* phrase, size_t max_len, char symbol, size_t target_len, ALIGN_MODE mode);


#ifdef __cplusplus
}
#endif


#endif

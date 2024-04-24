/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef __G_TIME_H
#define __G_TIME_H


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>


#ifdef USE_HAL_DRIVER
#   define TIME_MS_T uint32_t 
#else
#   define TIME_MS_T unsigned long long 
#endif


TIME_MS_T getMillis();


#ifdef __cplusplus
}
#endif


#endif

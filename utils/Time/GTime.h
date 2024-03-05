/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef __G_TIME_H
#define __G_TIME_H


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>


#ifdef USE_HAL_DRIVER
uint32_t getMillis();
#else
unsigned long long getMillis();
#endif


#ifdef __cplusplus
}
#endif


#endif

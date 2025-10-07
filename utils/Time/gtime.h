/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef __G_TIME_H
#define __G_TIME_H


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>


typedef uint64_t g_time_t;


g_time_t getMillis();

g_time_t getMicroseconds();


#ifdef __cplusplus
}
#endif


#endif

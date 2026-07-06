/* Copyright © 2024 Georgy E. All rights reserved. */

#ifndef _G_VERSION_H_
#define _G_VERSION_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>


#define GVERSION_OK     (0)
#define GVERSION_HIGHER (1)
#define GVERSION_LOWER  (-1)
#define GVERSION_ERROR  (-2)


typedef struct _gversion_t {
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
} gversion_t;


int gversion_compare(const gversion_t* first, const gversion_t* second);
char* gversion_to_string(const gversion_t* version);
bool gversion_from_string(const char* str, const unsigned size, gversion_t* version);


#ifdef __cplusplus
}
#endif


#endif
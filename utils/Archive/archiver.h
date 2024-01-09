/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef __ARCHIVER_H
#define __ARCHIVER_H


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>


// TODO: Archiver for records in flash memory
void zip(uint8_t* src, uint8_t* arch, uint32_t src_len, uint32_t arch_len);
void unzip(uint8_t* arch, uint8_t* dst, uint32_t arch_len, uint32_t dst_len);


#ifdef __cplusplus
}
#endif


#endif

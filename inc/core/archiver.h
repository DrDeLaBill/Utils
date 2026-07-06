/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef __ARCHIVER_H
#define __ARCHIVER_H


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stddef.h>


typedef enum _archiver_status_t {
	ARCHIVER_OK = 0,
	ARCHIVER_BAD_ARGS,
	ARCHIVER_NO_SPACE,
	ARCHIVER_BAD_FRAME,
	ARCHIVER_BAD_CRC,
	ARCHIVER_UNSUPPORTED_ALGO
} archiver_status_t;


typedef enum _archiver_algo_t {
	ARCHIVER_ALGO_STORE = 0
} archiver_algo_t;


uint16_t archiver_crc16_ccitt(const uint8_t* data, uint32_t len);
uint32_t archiver_min_frame_size(void);


archiver_status_t zip(const uint8_t* src, uint8_t* arch, uint32_t src_len, uint32_t arch_len);
archiver_status_t unzip(const uint8_t* arch, uint8_t* dst, uint32_t arch_len, uint32_t dst_len);


#ifdef __cplusplus
}
#endif


#endif

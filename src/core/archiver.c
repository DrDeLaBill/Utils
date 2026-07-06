/* Copyright © 2023 Georgy E. All rights reserved. */

#include "archiver.h"


#include <string.h>
#include <stdint.h>


#define ARCHIVER_MAGIC   (0x5241u) /* "AR" in little-endian frame */
#define ARCHIVER_VERSION (1u)

#define FRAME_OFF_MAGIC      (0u)
#define FRAME_OFF_VERSION    (2u)
#define FRAME_OFF_ALGO       (3u)
#define FRAME_OFF_SRC_LEN    (4u)
#define FRAME_OFF_PAYLOAD    (8u)
#define FRAME_OFF_CRC16      (12u)
#define FRAME_HEADER_SIZE    (14u)


static void _write_u16_le(uint8_t* dst, uint16_t value)
{
    dst[0] = (uint8_t)(value & 0xFFu);
    dst[1] = (uint8_t)((value >> 8) & 0xFFu);
}

static void _write_u32_le(uint8_t* dst, uint32_t value)
{
    dst[0] = (uint8_t)(value & 0xFFu);
    dst[1] = (uint8_t)((value >> 8) & 0xFFu);
    dst[2] = (uint8_t)((value >> 16) & 0xFFu);
    dst[3] = (uint8_t)((value >> 24) & 0xFFu);
}

static uint16_t _read_u16_le(const uint8_t* src)
{
    return (uint16_t)(src[0] | ((uint16_t)src[1] << 8));
}

static uint32_t _read_u32_le(const uint8_t* src)
{
    return ((uint32_t)src[0]) |
           ((uint32_t)src[1] << 8) |
           ((uint32_t)src[2] << 16) |
           ((uint32_t)src[3] << 24);
}


uint16_t archiver_crc16_ccitt(const uint8_t* data, uint32_t len)
{
    if (!data && len > 0u) {
        return 0u;
    }

    uint16_t crc = 0xFFFFu;
    for (uint32_t i = 0u; i < len; i++) {
        crc ^= (uint16_t)((uint16_t)data[i] << 8);
        for (uint8_t b = 0u; b < 8u; b++) {
            if ((crc & 0x8000u) != 0u) {
                crc = (uint16_t)((crc << 1) ^ 0x1021u);
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

uint32_t archiver_min_frame_size(void)
{
    return FRAME_HEADER_SIZE;
}


archiver_status_t zip(const uint8_t* src, uint8_t* arch, uint32_t src_len, uint32_t arch_len)
{
    if (!arch || (!src && src_len > 0u)) {
        return ARCHIVER_BAD_ARGS;
    }

    uint32_t frame_size = FRAME_HEADER_SIZE + src_len;
    if (arch_len < frame_size) {
        return ARCHIVER_NO_SPACE;
    }

    _write_u16_le(&arch[FRAME_OFF_MAGIC], ARCHIVER_MAGIC);
    arch[FRAME_OFF_VERSION] = (uint8_t)ARCHIVER_VERSION;
    arch[FRAME_OFF_ALGO] = (uint8_t)ARCHIVER_ALGO_STORE;
    _write_u32_le(&arch[FRAME_OFF_SRC_LEN], src_len);
    _write_u32_le(&arch[FRAME_OFF_PAYLOAD], src_len);

    if (src_len > 0u) {
        memcpy(&arch[FRAME_HEADER_SIZE], src, src_len);
    }

    uint16_t crc = archiver_crc16_ccitt(&arch[FRAME_HEADER_SIZE], src_len);
    _write_u16_le(&arch[FRAME_OFF_CRC16], crc);

    return ARCHIVER_OK;
}

archiver_status_t unzip(const uint8_t* arch, uint8_t* dst, uint32_t arch_len, uint32_t dst_len)
{
    if (!arch || !dst) {
        return ARCHIVER_BAD_ARGS;
    }

    if (arch_len < FRAME_HEADER_SIZE) {
        return ARCHIVER_BAD_FRAME;
    }

    uint16_t magic = _read_u16_le(&arch[FRAME_OFF_MAGIC]);
    uint8_t version = arch[FRAME_OFF_VERSION];
    uint8_t algo = arch[FRAME_OFF_ALGO];
    uint32_t src_len = _read_u32_le(&arch[FRAME_OFF_SRC_LEN]);
    uint32_t payload_len = _read_u32_le(&arch[FRAME_OFF_PAYLOAD]);
    uint16_t crc_expected = _read_u16_le(&arch[FRAME_OFF_CRC16]);

    if (magic != ARCHIVER_MAGIC || version != ARCHIVER_VERSION) {
        return ARCHIVER_BAD_FRAME;
    }

    if (algo != (uint8_t)ARCHIVER_ALGO_STORE) {
        return ARCHIVER_UNSUPPORTED_ALGO;
    }

    if (payload_len != src_len) {
        return ARCHIVER_BAD_FRAME;
    }

    if ((uint64_t)FRAME_HEADER_SIZE + (uint64_t)payload_len > (uint64_t)arch_len) {
        return ARCHIVER_BAD_FRAME;
    }

    if (dst_len < src_len) {
        return ARCHIVER_NO_SPACE;
    }

    uint16_t crc_actual = archiver_crc16_ccitt(&arch[FRAME_HEADER_SIZE], payload_len);
    if (crc_expected != crc_actual) {
        return ARCHIVER_BAD_CRC;
    }

    if (src_len > 0u) {
        memcpy(dst, &arch[FRAME_HEADER_SIZE], src_len);
    }

    return ARCHIVER_OK;
}
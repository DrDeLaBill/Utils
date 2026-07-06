/* Copyright © 2026 Georgy E. All rights reserved. */

#include <gtest/gtest.h>

#include <cstring>

#include "archiver.h"


TEST(ArchiverTest, ZipAndUnzipRoundtripStore)
{
    uint8_t src[8] = {1,2,3,4,5,6,7,8};
    uint8_t arch[64] = {};
    uint8_t dst[8] = {};

    ASSERT_EQ(zip(src, arch, (uint32_t)sizeof(src), (uint32_t)sizeof(arch)), ARCHIVER_OK);
    ASSERT_EQ(unzip(arch, dst, (uint32_t)sizeof(arch), (uint32_t)sizeof(dst)), ARCHIVER_OK);
    ASSERT_EQ(memcmp(src, dst, sizeof(src)), 0);
}


TEST(ArchiverTest, UnzipDetectsBadCrc)
{
    uint8_t src[6] = {10,20,30,40,50,60};
    uint8_t arch[64] = {};
    uint8_t dst[6] = {};

    ASSERT_EQ(zip(src, arch, (uint32_t)sizeof(src), (uint32_t)sizeof(arch)), ARCHIVER_OK);

    // Corrupt payload so CRC check must fail.
    arch[archiver_min_frame_size() + 1u] ^= 0xFFu;

    ASSERT_EQ(unzip(arch, dst, (uint32_t)sizeof(arch), (uint32_t)sizeof(dst)), ARCHIVER_BAD_CRC);
}


TEST(ArchiverTest, UnzipFailsWhenDstIsTooSmall)
{
    uint8_t src[8] = {1,2,3,4,5,6,7,8};
    uint8_t arch[64] = {};
    uint8_t dst_small[4] = {};

    ASSERT_EQ(zip(src, arch, (uint32_t)sizeof(src), (uint32_t)sizeof(arch)), ARCHIVER_OK);
    ASSERT_EQ(unzip(arch, dst_small, (uint32_t)sizeof(arch), (uint32_t)sizeof(dst_small)), ARCHIVER_NO_SPACE);
}

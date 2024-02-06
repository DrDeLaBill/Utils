/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef _VARIABLES_H_
#define _VARIABLES_H_


#include <memory>


namespace utl
{
template<class dst_t>
std::shared_ptr<dst_t[]> deserialize(const uint8_t* src, unsigned length = 1)
{
    if (!src || length == 0) {
        return nullptr;
    }

    std::shared_ptr<dst_t[]> dst(
        new dst_t[length],
        [] (dst_t* arr) { delete [] arr; }
        );
    for (unsigned i = 0; i < length; i++) {
        for (unsigned j = sizeof(dst_t); j > 0; j--) {
            dst[i] <<= 8;
            dst[i] |= const_cast<uint8_t*>(src)[i * sizeof(dst_t) + (j - 1)];
        }
    }

    return dst;
}

template<class src_t>
std::shared_ptr<uint8_t[]> serialize(const src_t* src, unsigned length = 1)
{
    if (!src || length == 0) {
        return nullptr;
    }

    std::shared_ptr<uint8_t[]> dst(
        new uint8_t[length * sizeof(src_t)],
        [] (uint8_t* arr) { delete [] arr; }
        );
    for (unsigned i = 0; i < length; i++) {
        src_t tmp = src[i];
        for (unsigned j = 0; j < sizeof(src_t); j++) {
            dst[i * sizeof(src_t) + j] = (tmp & 0xFF);
            tmp >>= 8;
        }
    }

    return dst;
}
}

#endif

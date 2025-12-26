 /* Copyright © 2025 Georgy E. All rights reserved. */

#ifndef _G_SORT_HPP_
#define _G_SORT_HPP_


#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>


template<class T>
void gsort(T* buf, const size_t size)
{
    if (size < 2) {
        return;
    }
    for (size_t i = 0; i < size; i++) {
        bool sorted = false;
        for (size_t j = 0; j < size - 1; j++) {
            if (buf[j] > buf[j+1]) {
                std::swap(buf[j], buf[j+1]);
                sorted = true;
            }
        }
        if (!sorted) {
            break;
        }
    }
}

template<class T>
T gmedian(const T* buf, const size_t size)
{
    T tmpBuf[size] = {};
    memcpy((uint8_t*)tmpBuf, (uint8_t*)buf, size * sizeof(buf[0]));
    gsort(tmpBuf, size);
    return tmpBuf[size / 2];
}


#endif // #ifndef _G_SORT_HPP_
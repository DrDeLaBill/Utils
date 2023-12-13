/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once


#include <cstdio>


namespace utl
{

#if DEBUG

    template<class... Args>
    void printTagLog(const char* format)
    {
        printf("%09lu->%s:\t", HAL_GetTick(), MODULE_TAG);
        printMessage<Args...>(format);
        printf("\n");
    }

    // template<>
    // void printTagLog(const char* format)
    // {
    //     printf("%09lu->%s:\t", HAL_GetTick(), MODULE_TAG);
    //     printMessage<Args...>(format);
    //     printf("\n");
    // }

    template<class... Args>
    void printLog(const char* format)
    {
        printMessage<Args...>(format);
    }

#else

    template<class... Args>
    void printTagLog(const char*) { }

    template<class... Args>
    void printLog(const char*) { }

#endif

    template<class... Args>
    void printMessage(const char* format)
    {
        printf(format, Args...); 
    }

    template<>
    void printMessage(const char* format)
    {
        printf(format); 
    }

}
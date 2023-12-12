/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once


#include <cstdio>


namespace utl
{

#if DEBUG

    template<class... Args>
    void printTagLog(char* format)
    {
        printf("%09lu->%s:\t", HAL_GetTick(), MODULE_TAG);
        printMessage<Args...>(format);
        printf("\n");
    }

    // template<>
    // void printTagLog(char* format)
    // {
    //     printf("%09lu->%s:\t", HAL_GetTick(), MODULE_TAG);
    //     printMessage<Args...>(format);
    //     printf("\n");
    // }

    template<class... Args>
    void printLog(char* format)
    {
        printMessage<Args...>(format);
    }

#else

    template<class... Args>
    void printTagLog(char* format) { }

    template<class... Args>
    void printLog(char* format) { }

#endif

    template<class... Args>
    void printMessage(char* format)
    {
        printf(format, Args...); 
    }

    template<>
    void printMessage(char* format)
    {
        printf(format); 
    }

}
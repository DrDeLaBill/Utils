/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once


#include <cstdio>

#include "GTime.h"


namespace utl
{

    template<class... Args>
    void printMessage(const char* format, Args... args)
    {
        printf(format, args...); 
    }

    // template<>
    // void printMessage(const char* format)
    // {
    //     printf(format); 
    // }

#if _DEBUG || DEBUG

    template<class... Args>
    void printTagLog(const char* tag, const char* format, Args... args)
    {
        printMessage("%09lu->%s:\t", Time::getMillis(), tag);
        printMessage(format, args...);
        printMessage("\n");
    }

    template<class... Args>
    void printLog(const char* format, Args... args)
    {
        printMessage(format, args...);
    }

#else

    template<class... Args>
    void printTagLog(const char*) { }

    template<class... Args>
    void printLog(const char*) { }

#endif

}
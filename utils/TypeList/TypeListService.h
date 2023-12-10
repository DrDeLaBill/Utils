/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once


#include <cstddef>

#include "TypeListBuilder.h"


namespace utl 
{
    template<class... TypeList>
    constexpr std::size_t size(Private::TypeUnit<TypeList...>) {
    	return 1 + size(typename Private::Tail<Private::TypeUnit<TypeList...>>::TYPE{});
    }

    template<class H>
    constexpr std::size_t size(Private::TypeUnit<H, NullType>) {
    	return 1;
    }
    
    template<class H, class... TypeList>
    constexpr bool empty(Private::TypeUnit<H, TypeList...>) {
        return size(typename Private::TypeUnit<H, TypeList...>{}) == 0;
    }
    
    template<>
    constexpr bool empty(Private::TypeUnit<NullType>) {
        return true;
    }
    
    constexpr bool empty(NullType) {
        return true;
    }
}
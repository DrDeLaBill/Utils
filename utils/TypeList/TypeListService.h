/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once


#include <cstddef>

#include "TypeListBuilder.h"


namespace utl 
{
    template<class... TypeList>
    constexpr std::size_t size(unit_t<TypeList...>) {
    	return 1 + size(typename unit_t::TAIL{});
    }

    template<class Head>
    constexpr std::size_t size(unit_t<Head, null_type_t>) {
    	return 1;
    }

    // TODO: get head & tail types
    // template<class H, class... TypeList>
    // constexpr head_t head(Private::TypeUnit<H, TypeList...>) {
    //     return head_t<>
    // }
    
    template<class... TypeList>
    constexpr bool empty(typelist_t<TypeList...>) {
        return size(typename typelist_t<TypeList...>{}) == 0;
    }
    
    template<>
    constexpr bool empty(typelist_t<null_type_t>) {
        return true;
    }
    
    constexpr bool empty(null_type_t) {
        return true;
    }

    // TODO: операторы сравнения

	// TODO: https://github.com/ALSCode/FSM/blob/master/fsm/typelist.hpp
}
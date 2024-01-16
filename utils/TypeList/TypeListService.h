/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once


#include <cstddef>

#include "TypeListBuilder.h"


namespace utl 
{
    // Get type list length
    template<class... TypeList>
    constexpr std::size_t size(unit_t<TypeList...>) {
    	return 1 + size(typename unit_t<TypeList...>::TAIL{});
    }

    template<class Head>
    constexpr std::size_t size(unit_t<Head, null_type_t>) {
    	return 1;
    }
    
    template<class... TypeList>
    constexpr bool empty(unit_t<TypeList...> list) {
        return size(list) == 0;
    }
    
    template<>
    constexpr bool empty(unit_t<null_type_t>) {
        return true;
    }
    
    constexpr bool empty(null_type_t) {
        return true;
    }

    // TODO: операторы сравнения

	// TODO: https://github.com/ALSCode/FSM/blob/master/fsm/typelist.hpp
}

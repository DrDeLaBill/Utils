/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once

#include <variant>
//#include <type_traits>

#include "TypeListBuilder.h"

template<class... Types>
constexpr auto get_types_obj(utl::type_list_t<Types...>) {
    return std::variant<Types...>{};
}

namespace fsm
{
    struct TransitionTableBase { };

    template<class... T>
    struct TransitionTable : TransitionTableBase
    {
        static_assert(
            std::is_same_v<utl::remove_duplicates_t<T...>, utl::type_list_t<T...>>,
            "FSM repeated transitions"
        );

        // using transition_p = type_pack<T...>;

        using state_collection = utl::remove_duplicates_t<utl::type_list_t<typename T::source_t..., typename T::target_t...>>;

        using event_collection = utl::remove_duplicates_t<typename T::event_t...>;

        using state_v = decltype(get_types_obj(state_collection{}));
        using event_v = decltype(get_types_obj(event_collection{}));
        using transition_v = std::variant<T...>;
    };
};
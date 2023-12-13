/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once

#include <variant>
//#include <type_traits>

#include "TypeListBuilder.h"


namespace fsm
{
    struct TransitionTableBase { };

    template<class... T>
    struct TransitionTable : TransitionTableBase
    {
        static_assert(
            std::is_same_v<
                utl::variant_factory<typename utl::typelist_t<T...>::RESULT>,
                utl::variant_factory<typename utl::removed_duplicates_t<T...>::RESULT>
            >,
            "FSM repeated transitions"
        );

        using transition_p = utl::simple_list_t<T...>;

        using state_collection = typename utl::removed_duplicates_t<
            typename T::source_t..., 
            typename T::target_t...
        >::RESULT;
        using event_collection = typename utl::removed_duplicates_t<
            typename T::event_t...
        >::RESULT;

        using state_v = typename utl::variant_factory<state_collection>::VARIANT;
        using event_v = typename utl::variant_factory<event_collection>::VARIANT;
        using transition_v = std::variant<T...>;
    };
}
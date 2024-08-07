/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef _FSM_TRANSITION_TABLE_H_
#define _FSM_TRANSITION_TABLE_H_


#include <variant>

#include "TypeListBuilder.h"
#include "TypeListService.h"


namespace fsm
{
    struct TransitionTableBase { };

    template<class... T>
    struct TransitionTable : TransitionTableBase
    {
        static_assert(
            !utl::empty(typename utl::typelist_t<T...>::RESULT{}),
            "FSM empty transition table"
        );
        static_assert(
            std::is_same_v<
                typename utl::variant_factory<utl::typelist_t<T...>>::VARIANT,
                typename utl::variant_factory<utl::removed_duplicates_t<T...>>::VARIANT
            >,
            "FSM repeated transitions"
        );

        using transition_p = utl::simple_list_t<T...>;

        using state_collection = utl::removed_duplicates_t<
            typename T::source_t..., 
            typename T::target_t...
        >;
        using event_collection = utl::removed_duplicates_t<
            typename T::event_t...
        >;

        using state_v = typename utl::variant_factory<state_collection>::VARIANT;
        using event_v = typename utl::variant_factory<event_collection>::VARIANT;
        using transition_v = std::variant<T...>;
    };
}


#endif

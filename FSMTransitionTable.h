/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once

#include "TypeColleection.h"

namespace fsm
{
    struct FSMTransitionTableBase { };

    template<class... T>
    struct FSMTransitionTable : FSMTransitionTableBase
    {
        using assert_t = remove_duplicates_t<T...>;

        std::static_assert(
            std::is_same_v(assert_t, list_t<T...>),
            "FSM repeated transitions"
        );

        // using transition_p = type_pack<T...>;

        using state_collection = remove_duplicates_t<T::source_t..., T::target_t...>;

        using event_collection = remove_duplicates_t<T::event_t...>;

        // using state_v = decltype(get_var(state_collection{}));
        // using event_v = decltype(get_var(event_collection{}));
        // using transition_v = std::variant<T...>;
    }
}
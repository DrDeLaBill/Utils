/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once

#include <type_traits>

#include "FSMState.h"
#include "FSMEvent.h"
#include "FSMAction.h"
#include "FSMGuard.h"

namespace fsm
{
    struct TransitionBase { };

    template<
        class Source,
        class Event,
        class Target,
        class Action,
#if FSM_ENABLE_GUARD
        Guard trGuard = fsm::Guard::NO_GUARD,
#endif
        class = std::enable_if_t<
            std::is_base_of_v<StateBase, Source> &&
            std::is_base_of_v<EventBase, Event> &&
            std::is_base_of_v<StateBase, Target>
        >
    >
    struct Transition : TransitionBase
    {
        using source_t = Source;
        using event_t  = Event;
        using target_t = Target;
        using action_t = Action;

#if FSM_ENABLE_GUARD
        static const Guard guard = trGuard;
#endif
    };
}

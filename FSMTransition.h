/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once

// #include <type_traits>

#include "FSMState.h"
#include "FSMEvent.h"
#include "FSMAction.h"
#include "FSMGuard.h"

namespace fsm
{
    struct FSMTransitionBase { };

    template<
        class Source,
        class Event,
        class Target,
        class Action
        FSMGuard fsmGuard,
        class = 
            std::enable_if_t<
                std::is_base_of_v<FSMStateBase, Source> && 
                std::is_base_of_v<FSMEventBase, FSMEvent> &&
                std::is_base_of_v<FSMStateBase, Target>
            >
    >
    struct FSMTransition : FSMTransitionBase
    {
        using source_t = Source;
        using event_t = Event;
        using target_t = Target;
        using action_t = Action;

        static const Guard guard = fsmGuard;
    };
}
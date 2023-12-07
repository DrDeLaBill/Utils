/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once

namespace fsm
{
    struct FSMStateBase { };

    template<unsigned INDEX, class Action=void>
    struct FSMState : FSMStateBase
    {
        static const unsigned index = INDEX;
        using action_t = Action;
    };
}
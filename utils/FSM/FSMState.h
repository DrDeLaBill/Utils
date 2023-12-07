/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once

namespace fsm
{
    struct StateBase { };

    template<unsigned INDEX, class Action = void>
    struct State : StateBase
    {
        static const unsigned index = INDEX;
        using action_t = Action;
    };
};
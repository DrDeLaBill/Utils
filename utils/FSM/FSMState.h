/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef _FSM_STATE_H_
#define _FSM_STATE_H_


namespace fsm
{
    struct StateBase { };

    template<unsigned INDEX, class Action = void>
    struct State : StateBase
    {
        static const unsigned index = INDEX;
        using action_t = Action;
    };
}


#endif

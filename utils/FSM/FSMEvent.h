/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef _FSM_EVENT_H_
#define _FSM_EVENT_H_


namespace fsm
{
    struct EventBase { };

    template<unsigned INDEX, unsigned PRIORITY = 0>
    struct Event : EventBase
    {
        static constexpr unsigned index = INDEX;
        static constexpr unsigned priority = PRIORITY;
    };
}


#endif

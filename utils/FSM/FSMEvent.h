/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once

namespace fsm
{
    struct EventBase { };

    template<unsigned INDEX>
    struct Event : EventBase
    {
        static const unsigned index = INDEX;
    };
}
/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once

namespace fsm
{
    struct FSMEventBase { };

    template<unsigned INDEX>
    struct FSMEvent : FSMEventBase
    {
        static const unsigned index = INDEX;
    };
}
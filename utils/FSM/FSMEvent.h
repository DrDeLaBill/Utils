/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once

namespace fsm
{
    struct EventBase { };

    // TODO: template<unsigned INDEX, unsigned PRIORITY>
    template<unsigned INDEX>
    struct Event : EventBase
    {
        static constexpr unsigned index = INDEX;
        // TODO: static constexpr unsigned priority = PRIORITY;
    };

    // template<class... EventTypes>
    // constexpr utl::type_list_t<> get_event_typelist()
    // {

    // }
}
/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once


#define FSM_ENABLE_GUARD (false)


namespace fsm
{
    enum class Guard : unsigned
    {
        NO_GUARD,
        NO_STATE
    };

    struct GuardEqual
    {
        constexpr bool operator()(const Guard& lhs, const Guard& rhs) const {
            return lhs == rhs;
        }
    };
}

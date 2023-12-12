/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once

namespace fsm
{
    enum class Guard : unsigned
    {
        NO_GUARD,
        NO_STATE
    };

    struct GuardEqual
    {
        constexpr bool operator()(const Guard& lhs, const Guard& rhs) {
            return lhs == rhs;
        }
    };
}
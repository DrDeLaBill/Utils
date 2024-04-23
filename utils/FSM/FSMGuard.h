/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef _FSM_GUARD_H_
#define _FSM_GUARD_H_


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


#endif

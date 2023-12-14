/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once


#include <cstdint>


namespace fsm
{
    struct key_t
    {
        uint32_t state_idx = 0;
        uint32_t event_idx = 0;
    };

    struct KeyEqual
    {
        constexpr bool operator()(const key_t& lhs, const key_t& rhs) const 
        {
            return (lhs.state_idx == rhs.state_idx) && (lhs.event_idx == rhs.event_idx);
        }
    };

    struct KeyHash
    {
        std::size_t operator()(const key_t& key) const
        {
            std::size_t hash_1 = std::hash<unsigned>{}(key.state_idx);
            std::size_t hash_2 = std::hash<unsigned>{}(key.event_idx);

            return hash_1 ^ (hash_2 << 1);
        }
    };
}
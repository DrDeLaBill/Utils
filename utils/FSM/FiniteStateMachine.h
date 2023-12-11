/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once


#include <cstdint>
#include <unordered_map>

#include "FSMTransitionTable.h"

#include "CircleBuffer.h"


namespace fsm
{
    struct key_t
    {
        uint32_t state_idx = 0;
        uint32_t event_idx = 0;
    };

    template<unsigned EVENT_STACK_SIZE, class TransitionTable>
    class FiniteStateMachine
    {
    private:
        using tuple_t = std::unordered_map<key_t, transition_v, keyHash, keyEqual>;
        using queue_t = utl::circle_buffer<EVENT_STACK_SIZE, event_v, Atomic>;

        key_t key;
        tuple_t transitions;
        queue_t events;
        state_v state;
        Guard guard = Guard::OFF;

    public:
        using transition_p = typename TransitionTable::transition_p;

        FiniteStateMachine()
        {
            set();
        }

        template<class... TrPack>
        void set()
        {

        }
    };
}
/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once


#include <cstdint>
#include <unordered_map>

#include "FSMGuard.h"
#include "FSMTransitionTable.h"

#include "CircleBuffer.h"


namespace fsm
{
    struct key_t
    {
        uint32_t state_idx = 0;
        uint32_t event_idx = 0;
    };

    template<class TrTable>
    class FiniteStateMachine
    {
    private:
        static constexpr unsigned EVENT_STACK_SIZE = 8;

        using tuple_t = std::unordered_map<key_t, TransitionTable::transition_v, keyHash, keyEqual>;
        using queue_t = utl::circle_buffer<EVENT_STACK_SIZE, TransitionTable::event_v, Atomic>;

        key_t key;
        tuple_t transitions;
        queue_t events;
        state_v state;
        Guard guard = Guard::OFF;

    public:
        using transition_p = typename TrTable::transition_p;

        FiniteStateMachine()
        {
            set_table(transition_p{});
        }

        template<class... Args>
        void state_action(const Args&... args)
        {
            state_v state_tmp_v{this->state};

            auto lamda = [&] (const auto& arg) {
                // using state_t = std::decay_t
            };
        }

    private:
        template<class... TrPacks>
        void set_table(TrPacks...)
        {
            (set_tuple(utl::TypeUnit<TrPacks>{}), ...);
        }

        template<class TrPack>
        void set_tuple(utl::TypeUnit<TrPack> trPack)
        {
            using transition = utl::head_t<TrPack>;

            using state_t = typename transition::source_t;
            using event_t = typename transition::event_t;
            Guard guard = transition::guard;

            key_t key;
            key.state_idx = state_t::index;
            key.event_idx = event_t::index;

            transitions.insert({key, transition{}});

            if (!this->key.state_idx) {
                this->key.state_idx = key.state_idx;
                this->guard = guard;
                this->state = state_t{};
            }
        }
    };
}
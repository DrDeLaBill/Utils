/* Copyright © 2023 Georgy E. All rights reserved. */

#pragma once


#include <cstdint>
#include <variant>
//#include <algorithm>
#include <unordered_map>

#include "FSMKey.h"
#include "FSMGuard.h"
#include "FSMEvent.h"
#include "FSMAction.h"
#include "FSMCharHash.h"
#include "FSMTransitionTable.h"

#include "CircleBuffer.h"


namespace fsm
{
    template<class TrTable>
    class FiniteStateMachine
    {
    public:
        using transition_pack = typename TrTable::transition_p;

        using transition_v = typename TrTable::transition_v;
        using state_v = typename TrTable::state_v;
        using event_v = typename TrTable::event_v;

    private:
        static constexpr unsigned EVENT_STACK_SIZE = 8;

        using tuple_t = 
            std::unordered_map<
                key_t, 
                transition_v, 
                KeyHash, 
                KeyEqual
                //std::allocator<std::pair<key_t, transition_v>>
            >;
        using queue_t = 
            utl::circle_buffer<
                EVENT_STACK_SIZE,
                event_v
            >;

        key_t key;
        tuple_t transitions;
        queue_t events;
        state_v current_state;
        Guard guard = Guard::NO_GUARD;

    public:
        using transition_p = typename TrTable::transition_p;

        FiniteStateMachine()
        {
            set_table(transition_p{});
        }

        template<unsigned EVENT_INDEX>
        void push_event(const Event<EVENT_INDEX>& event)
        {
            events.push_back(event);
        }

        // template<class... EventTypes>
        // void push_events(const utl::type_list_t<EventTypes...>&)
        // {
        //     for (unsigned i = 0; i < utl::size(utl::type_list_t<EventTypes...>); i++) {
        //         push_event<event::index>(utl::get_type_unit<eventTypes, i>());
        //     }
        // }

        void set_guard(const Guard& guard) // TODO: : guard(guard)
        {
            this->guard = guard;
        }

        template<class... Args>
        void state_action(const Args&... args)
        {
            state_v state_tmp_v{ this->current_state };

            auto lambda = [&](const auto& state) {
                using state_t = std::decay_t<decltype(state)>;
                using functor_t = typename state_t::action_t;

                if constexpr (!std::is_same_v<functor_t, void>) {
                    functor_t{}(args...);
                }
            };

            std::visit(lambda, state_tmp_v);
        }

        void proccess(void)
        {
            state_action();

            key_t tmpKey;
            tmpKey.state_idx = this->key.state_idx;
            auto it = transitions.begin();

            for (unsigned i = 0; i < events.size(); i++) {
                auto eventVariant = events.front();
                auto lambda = [&](const auto& targetEvent) {
                    using event_t = std::decay_t<decltype(targetEvent)>;
                    tmpKey.event_idx = event_t::index;
                    it = transitions.find(tmpKey);
                };

                std::visit(lambda, eventVariant);

                if (it != transitions.end()) {
                    events.pop();
                    on_event_invoke(tmpKey);
                    return;
                } else {
                    events.push(eventVariant);
                    events.pop();
                }
            }
        }

        template<
            class TEvent,
            class=std::enable_if_t<std::is_base_of_v<EventBase, TEvent>>
        >
        void on_event(const TEvent& event)
        {
            key_t tmpKey;
            tmpKey.event_idx = event.index;
            tmpKey.state_idx = key.state_idx;
            on_event_invoke(tmpKey);
        }

    private:
        void on_event_invoke(key_t& targetKey)
        {
            transition_v targetVariant = transitions[targetKey];

            key_t& tmpKey = this->key;
            Guard& tmpGuard = this->guard;
            state_v& tmpState = this->current_state;

            auto lambda = [&](const auto& targetVariant) {
                using transition_v = std::decay_t<decltype(targetVariant)>;
                using action_t = typename transition_v::action_t;

                if (!GuardEqual{}(tmpGuard, transition_v::guard)) {
                    return;
                }

                using target_t = typename transition_v::target_t;

                tmpKey.state_idx = target_t::index;
                tmpState = target_t{};

                action_t{}();
            };

            std::visit(lambda, targetVariant);
        }

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
                this->current_state = state_t{};
            }
        }
    };
}
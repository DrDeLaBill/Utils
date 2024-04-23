/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef _FINITE_STATE_MACHINE_H_
#define _FINITE_STATE_MACHINE_H_


#include <cstdint>
#include <variant>
#include <unordered_map>

#include "FSMKey.h"
#include "FSMGuard.h"
#include "FSMEvent.h"
#include "FSMState.h"
#include "FSMAction.h"
#include "FSMCharHash.h"
#include "FSMTransition.h"
#include "FSMTransitionTable.h"

#include "CircleBuffer.h"
#include "TypeListBuilder.h"
#include "TypeListService.h"

#include "bmacro.h"


#define FSM_CREATE_STATE(name, action)   using name = fsm::State<fsm::charHash(#name), action>
#define FSM_CREATE_EVENT(name, priority) using name = fsm::Event<fsm::charHash(#name), priority>


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

        void push_event(const event_v& event)
        {
            events.push_back(event);
        }

        template<class... Event>
        void push_events(Event...)
        {
            (events.push_back(Event{}), ...);
        }

        void clear_events()
        {
            events.clear();
        }

        void set_guard(const Guard& guard)
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

            if (events.empty()) {
                return;
            }

            auto it              = transitions.begin();
            unsigned maxPriority = 0;
            unsigned index       = 0;
            for (unsigned i = 0; i < events.count(); i++) {
                key_t tmpKey{ this->key.state_idx, 0 };
                auto eventVariant = events[i];

                auto lambda = [&](const auto& targetEvent) {
                    using event_t    = std::decay_t<decltype(targetEvent)>;
                    tmpKey.event_idx = event_t::index;
                    it               = transitions.find(tmpKey);

                    if (it == transitions.end()) {
                        return;
                    }

                    if(event_t::priority > maxPriority) {
                        maxPriority = event_t::priority;
                        index       = i;
                    }
                };
                std::visit(lambda, eventVariant);
            }

            auto lambda = [&](const auto& targetEvent) {
                using event_t = std::decay_t<decltype(targetEvent)>;
                key_t tmpKey{ this->key.state_idx, event_t::index };
                on_event_invoke(tmpKey);
            };

            std::visit(lambda, events[index]);
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

        bool is_state(state_v target)
        {
            bool result = false;

            auto lambda_i = [&] (const auto& state_i) {
                auto lambda_j = [&] (const auto& state_j) {
                    result = state_i.index == state_j.index;
                };
                std::visit(lambda_j, current_state);
            };

            std::visit(lambda_i, target);

            return result;
        }

    private:
        void on_event_invoke(key_t& targetKey)
        {
            transition_v targetVariant = transitions[targetKey];

            key_t& tmpKey = this->key;
            Guard& tmpGuard = this->guard;
            state_v& tmpState = this->current_state;

            auto lambda = [&](const auto& targetVariant) {
                using event_transition_v = std::decay_t<decltype(targetVariant)>;
                using action_t = typename event_transition_v::action_t;

                const Guard trGuard = event_transition_v::guard;

                if (!GuardEqual{}(tmpGuard, trGuard)) {
                    return;
                }

                using target_t = typename event_transition_v::target_t;

                tmpKey.state_idx = target_t::index;
                tmpState = target_t{};

                action_t{}();
            };

            std::visit(lambda, targetVariant);
        }

        template<class... TrPacks>
        void set_table(utl::simple_list_t<TrPacks...>)
        {
            (set_tuple(utl::getType<TrPacks>{}), ...);
        }

        template<class TrPack>
        void set_tuple(utl::getType<TrPack> trPack)
        {
            using transition = typename decltype(trPack)::TYPE;

            using state_t = typename transition::source_t;
            using event_t = typename transition::event_t;
            Guard tmpGuard = transition::guard;

            key_t tmpKey;
            tmpKey.state_idx = state_t::index;
            tmpKey.event_idx = event_t::index;

            transitions.insert({tmpKey, transition{}});

            if (!this->key.state_idx) {
                this->key.state_idx = tmpKey.state_idx;
                this->guard = tmpGuard;
                this->current_state = state_t{};
            }
        }
    };
}


#endif

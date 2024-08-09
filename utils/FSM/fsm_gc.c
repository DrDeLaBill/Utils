/* Copyright © 2024 Georgy E. All rights reserved. */

#include "fsm_gc.h"

#include <string.h>

#include "bmacro.h"


static size_t _fsm_gc_events_count = 1;


void fsm_gc_init(fsm_gc_t* fsm, fsm_gc_transition_t* table, unsigned size)
{
    if (!fsm) {
        BEDUG_ASSERT(false, "Empty FSM pointer");
        return;
    }
    if (!size || !table) {
        BEDUG_ASSERT(!fsm->_e_fsm_tt, "Empty FSM transition table");
        fsm->_e_fsm_tt = true;
        return;
    }
    fsm->_initialized = true;
    fsm->_table       = table;
    fsm->_table_size  = size;
    fsm->_state       = fsm->_table[0].source;
    for (unsigned i = 0; i < fsm->_table_size; i++) {
        fsm->_table[i].event->index = ++_fsm_gc_events_count;
    }
}

void fsm_gc_proccess(fsm_gc_t* fsm) 
{
    if (!fsm) {
        BEDUG_ASSERT(false, "Empty FSM pointer");
        return;
    }
    if (!fsm->_initialized) {
    	BEDUG_ASSERT(!fsm->_fsm_not_i, "FSM has not initialized");
        BEDUG_ASSERT(!fsm->_fsm_not_i, "FSM state and event must not be NULL");
        fsm->_fsm_not_i = true;
    	return;
    }

    bool is_transition = false;
	uint32_t table_idx = 0;
	size_t event_idx   = 0;
	size_t event_prio  = 0;
    if (fsm->_initialized && fsm->_state && fsm->_table) {
        fsm->_state->state();
        for (unsigned i = 0; i < fsm->_events_count; i++) {
            fsm_gc_event_t event = fsm->_events[i];
            for (unsigned j = 0; j < fsm->_table_size; j++) {
                fsm_gc_transition_t tr = fsm->_table[j]; \
                if (!(fsm->_state->state == tr.source->state && event.index == tr.event->index)) {
                	continue;
                }
				if (!is_transition ||
					event_prio < event.priority
				) {
					event_idx  = i;
					table_idx  = j;
					event_prio = event.priority;
				}
				is_transition = true;
            }
        } 

        if (!is_transition) {
        	return;
        }

		fsm->_state = fsm->_table[table_idx].target;
		fsm->_events_count--;
		memset((void*)&fsm->_events[event_idx], 0, sizeof(fsm->_events[event_idx]));

		for (unsigned i = event_idx; i < fsm->_events_count; i++) {
			fsm->_events[i] = fsm->_events[i + 1];
		}
		if (fsm->_table[table_idx].action) {
			fsm->_table[table_idx].action();
		}
    }
}

void fsm_gc_push_event(fsm_gc_t* fsm, fsm_gc_event_t* event)
{
    if (!fsm) {
        BEDUG_ASSERT(false, "Empty FSM pointer");
        return;
    }
    if (!event) {
        BEDUG_ASSERT(false, "Empty FSM event pointer");
        return;
    }
    if (!fsm->_initialized) {
    	BEDUG_ASSERT(!fsm->_fsm_not_i, "FSM has not initialized");
        fsm->_fsm_not_i = true;
    	return;
    }
    if (fsm->_events_count >= __arr_len(fsm->_events) - 1) {
        for (unsigned i = 0; i < __arr_len(fsm->_events) - 1; i++) {
            fsm->_events[i] = fsm->_events[i + 1];
        }
        fsm->_events_count = __arr_len(fsm->_events) - 1;
    }
    fsm->_events[fsm->_events_count++] = *event;
}

void fsm_gc_clear(fsm_gc_t* fsm)
{
    if (!fsm) {
        BEDUG_ASSERT(false, "Empty FSM pointer");
        return;
    }
    if (!fsm->_initialized) {
    	BEDUG_ASSERT(!fsm->_fsm_not_i, "FSM has not initialized");
        fsm->_fsm_not_i = true;
    	return;
    }
	memset(fsm->_events, 0, sizeof(fsm->_events));
	fsm->_events_count = 0;
}

bool fsm_gc_is_state(fsm_gc_t* fsm, fsm_gc_state_t* state)
{
    if (!fsm) {
        BEDUG_ASSERT(false, "Empty FSM pointer");
        return false;
    }
    if (!state) {
        BEDUG_ASSERT(false, "Empty FSM state pointer");
        return false;
    }
    if (!fsm->_initialized) {
    	BEDUG_ASSERT(!fsm->_fsm_not_i, "FSM has not initialized");
        fsm->_fsm_not_i = true;
    	return false;
    }
	return fsm->_state->state == state->state;
}

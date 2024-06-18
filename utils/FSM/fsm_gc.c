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
        BEDUG_ASSERT(false, "Empty FSM transition table");
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
    BEDUG_ASSERT(fsm->_initialized, "FSM has not initialized");
    if (fsm->_initialized && fsm->_state && fsm->_table) {
        fsm->_state->state();
        for (unsigned i = 0; i < fsm->_events_count; i++) {
            fsm_gc_event_t event = fsm->_events[i];
            for (unsigned j = 0; j < fsm->_table_size; j++) {
                fsm_gc_transition_t tr = fsm->_table[j]; \
                if (fsm->_state->state == tr.source->state && event.index == tr.event->index) {
                    fsm->_state = tr.target;
                    fsm->_events_count--;
                    for (unsigned k = 0; k < fsm->_events_count; k++) {
                        fsm->_events[k] = fsm->_events[k + 1];
                    }
                    i = fsm->_events_count;
                    j = fsm->_table_size;
                    break;
                }
            }
        } 
    } else {
        BEDUG_ASSERT(false, "FSM state and event must not be NULL");
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
    BEDUG_ASSERT(fsm->_initialized, "FSM has not initialized");
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
    BEDUG_ASSERT(fsm->_initialized, "FSM has not initialized");
	memset(fsm->_events, 0, sizeof(fsm->_events));
}

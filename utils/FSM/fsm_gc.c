/* Copyright © 2024 Georgy E. All rights reserved. */

#include "fsm_gc.h"

#include "utils.h"
#include "bmacro.h"


void fsm_gc_init(fsm_gc_t* fsm, fsm_gc_transition_t* table, unsigned size)
{
    if (!size) {
        BEDUG_ASSERT(false, "Empty fsm transition table");
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

void fsm_gc_push_event(fsm_gc_t* fsm, fsm_gc_event_t* event)
{
    if (fsm->_events_count >= __arr_len(fsm->_events) - 1) {
        for (unsigned i = 0; i < __arr_len(fsm->_events) - 1; i++) {
            fsm->_events[i] = fsm->_events[i + 1];
        }
        fsm->_events_count = __arr_len(fsm->_events) - 1;
    }
    fsm->_events[fsm->_events_count++] = *event;
}

void fsm_gc_proccess(fsm_gc_t* fsm) 
{
    BEDUG_ASSERT(fsm->_initialized, "fsm has not initilazed");
    if (fsm->_state && fsm->_table) {
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
        BEDUG_ASSERT(false, "fsm state and event must no be NULL");
    }
}
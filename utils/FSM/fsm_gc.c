/* Copyright © 2024 Georgy E. All rights reserved. */

#include "fsm_gc.h"

#include <string.h>

#include "glog.h"
#include "bmacro.h"



#ifdef FSM_GC_BEDUG

const char FSM_GC_TAG[] = "FSMc";

#endif

size_t _fsm_gc_events_iterator = 1;


void fsm_gc_init(fsm_gc_t* fsm, fsm_gc_transition_t* table, unsigned size)
{
    if (!fsm) {
#ifdef FSM_GC_BEDUG
        BEDUG_ASSERT(false, "Empty FSM pointer");
#endif
        return;
    }
    if (!size || !table) {
#ifdef FSM_GC_BEDUG
        BEDUG_ASSERT(!fsm->_e_fsm_tt, "Empty FSM transition table");
        fsm->_e_fsm_tt = true;
#endif
        return;
    }
    fsm->_initialized = true;
    fsm->_table       = table;
    fsm->_table_size  = size;
    fsm->_state       = fsm->_table[0].source;
    for (unsigned i = 0; i < fsm->_table_size; i++) {
		fsm->_table[i].event->index = ++_fsm_gc_events_iterator;
	}

#ifdef FSM_GC_BEDUG
    printTagLog(FSM_GC_TAG, "\"%s\" has been initialized", fsm->_name);
#endif
}

void fsm_gc_process(fsm_gc_t* fsm) 
{
    if (!fsm) {
#ifdef FSM_GC_BEDUG
        BEDUG_ASSERT(false, "Empty FSM pointer");
#endif
        return;
    }
    if (!fsm->_initialized || !fsm->_state || !fsm->_table) {
#ifdef FSM_GC_BEDUG
        BEDUG_ASSERT(!fsm->_fsm_not_i, "FSM has not initialized");
        BEDUG_ASSERT(!fsm->_fsm_not_i, "FSM state, table and event must not be NULL");
        fsm->_fsm_not_i = true;
#endif
        return;
    }

    fsm->_state->state();

    bool is_transition = false;
    uint32_t table_idx = 0;
    size_t event_idx = 0;
    size_t event_prio = 0;

    for (unsigned i = 0; i < fsm->_events_count; i++) {
        fsm_gc_event_t event = fsm->_events[i];
        for (unsigned j = 0; j < fsm->_table_size; j++) {
            fsm_gc_transition_t tr = fsm->_table[j];
            if (fsm->_state->state == tr.source->state && event.index == tr.event->index) {
                if (!is_transition || event_prio < event.priority) {
                    event_idx = i;
                    table_idx = j;
                    event_prio = event.priority;
                }
                is_transition = true;
            }
        }
    }

    if (!is_transition) {
        return;
    }

#ifdef FSM_GC_BEDUG
    printTagLog(
        FSM_GC_TAG,
        "\"%s\" transition: %s{%s} -> %s",
        fsm->_name,
        fsm->_table[table_idx].source->_name,
        fsm->_table[table_idx].event->_name,
        fsm->_table[table_idx].target->_name
    );
#endif

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

void fsm_gc_push_event(fsm_gc_t* fsm, fsm_gc_event_t* event)
{
    if (!fsm) {
#ifdef FSM_GC_BEDUG
        BEDUG_ASSERT(false, "Empty FSM pointer");
#endif
        return;
    }
    if (!event) {
#ifdef FSM_GC_BEDUG
        BEDUG_ASSERT(false, "Empty FSM event pointer");
#endif
        return;
    }
    if (!fsm->_initialized) {
#ifdef FSM_GC_BEDUG
    	BEDUG_ASSERT(!fsm->_fsm_not_i, "FSM has not initialized");
        fsm->_fsm_not_i = true;
#endif
    	return;
    }
    if (fsm->_events_count >= __arr_len(fsm->_events) - 1) {
        for (unsigned i = 0; i < __arr_len(fsm->_events) - 1; i++) {
            fsm->_events[i] = fsm->_events[i + 1];
        }
        fsm->_events_count = __arr_len(fsm->_events) - 1;
    }
    fsm->_events[fsm->_events_count++] = *event;


#ifdef FSM_GC_BEDUG
	printTagLog(
		FSM_GC_TAG, \
		"\"%s\" push event: %s",
		fsm->_name,
		event->_name
	);
#endif
}

void fsm_gc_clear(fsm_gc_t* fsm)
{
    if (!fsm) {
#ifdef FSM_GC_BEDUG
        BEDUG_ASSERT(false, "Empty FSM pointer");
#endif
        return;
    }
    if (!fsm->_initialized) {
#ifdef FSM_GC_BEDUG
    	BEDUG_ASSERT(!fsm->_fsm_not_i, "FSM has not initialized");
        fsm->_fsm_not_i = true;
#endif
    	return;
    }
	memset(fsm->_events, 0, sizeof(fsm->_events));
	fsm->_events_count = 0;

#ifdef FSM_GC_BEDUG
	printTagLog(
		FSM_GC_TAG, \
		"\"%s\" clear",
		fsm->_name
	);
#endif
}

bool fsm_gc_is_state(fsm_gc_t* fsm, fsm_gc_state_t* state)
{
    if (!fsm) {
#ifdef FSM_GC_BEDUG
        BEDUG_ASSERT(false, "Empty FSM pointer");
#endif
        return false;
    }
    if (!state) {
#ifdef FSM_GC_BEDUG
        BEDUG_ASSERT(false, "Empty FSM state pointer");
#endif
        return false;
    }
    if (!fsm->_initialized) {
#ifdef FSM_GC_BEDUG
    	BEDUG_ASSERT(!fsm->_fsm_not_i, "FSM has not initialized");
        fsm->_fsm_not_i = true;
#endif
    	return false;
    }
	return fsm->_state->state == state->state;
}

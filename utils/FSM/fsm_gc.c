/* Copyright © 2025 Georgy E. All rights reserved. */

#include "fsm_gc.h"

#include <string.h>

#include "glog.h"
#include "bmacro.h"



#ifdef FSM_GC_BEDUG

static const char FSM_GC_TAG[] = "FSMc";
static const char EMPTY[] = "NULL";
static bool disable_all_messages = false;

#endif

static size_t _fsm_gc_events_iterator = 1;



#ifdef FSM_GC_BEDUG
static bool _log_enabled(const fsm_gc_t* fsm)
{
    if (!fsm) {
        BEDUG_ASSERT(disable_all_messages, "Empty FSM pointer");
        return false;
    }
    return fsm->_enable_msg && !disable_all_messages;
}
#endif

static bool _check_initialized(fsm_gc_t* const fsm)
{
    if (!fsm) {
#ifdef FSM_GC_BEDUG
        BEDUG_ASSERT(disable_all_messages, "Empty FSM pointer");
#endif
        return false;
    }
    if (!fsm->_initialized) {
#ifdef FSM_GC_BEDUG
        BEDUG_ASSERT(!_log_enabled(fsm) || !fsm->_fsm_not_i, "FSM has not initialized");
        BEDUG_ASSERT(!_log_enabled(fsm) || !fsm->_fsm_not_i, "FSM state_f, table and event must not be NULL");
        fsm->_fsm_not_i = true;
#endif
        return false;
    }
    return true;
}


void fsm_gc_init(fsm_gc_t* fsm, fsm_gc_transition_t* table, unsigned size)
{
    if (!fsm) {
#ifdef FSM_GC_BEDUG
        BEDUG_ASSERT(disable_all_messages, "Empty FSM pointer");
#endif
        return;
    }

    fsm->_initialized = false;

    if (!size || !table) {
#ifdef FSM_GC_BEDUG
        BEDUG_ASSERT(!_log_enabled(fsm) || !fsm->_e_fsm_tt, "Empty FSM transition table");
        fsm->_e_fsm_tt = true;
#endif
        return;
    }

    fsm->_table      = table;
    fsm->_table_size = size;
    fsm->_state      = fsm->_table[0].source;
    fsm->_events_cnt = 0;

#ifdef FSM_GC_BEDUG
    // Check repeated and broken transitions
    for (unsigned i = 0; i < fsm->_table_size; i++) {
        if (!_log_enabled(fsm)) {
            continue;
        }
        for (unsigned j = i + 1; j < fsm->_table_size; j++) {
            if (fsm->_table[i].source && fsm->_table[j].source) {
                if (fsm->_table[i].source != fsm->_table[j].source &&
                    fsm->_table[i].source->state == fsm->_table[j].source->state
                ) {
                    printTagLog(
                        FSM_GC_TAG, 
                        "WARNING! \"%s\" has matches functions states %s{0x%08X} = %s{0x%08X}",
                        fsm->_name,
                        fsm->_table[i].source->_name,
                        (size_t)(size_t*)fsm->_table[i].source,
                        fsm->_table[j].source->_name,
                        (size_t)(size_t*)fsm->_table[j].source
                    );
                }
            }
            if (fsm->_table[i].target && fsm->_table[j].target) {
                if (fsm->_table[i].target != fsm->_table[j].target &&
                    fsm->_table[i].target->state == fsm->_table[j].target->state
                ) {
                    printTagLog(
                        FSM_GC_TAG, 
                        "WARNING! \"%s\" has matches functions states %s{0x%08X} = %s{0x%08X}",
                        fsm->_name,
                        fsm->_table[i].target->_name,
                        (size_t)(size_t*)fsm->_table[i].target,
                        fsm->_table[j].target->_name,
                        (size_t)(size_t*)fsm->_table[j].target
                    );
                }
            }
            if (fsm->_table[i].event && fsm->_table[j].event) {
                if (fsm->_table[i].event != fsm->_table[j].event &&
                    fsm->_table[i].event->index == fsm->_table[j].event->index
                ) {
                    printTagLog(
                        FSM_GC_TAG, 
                        "WARNING! \"%s\" has matches functions events %s{idx-%u} = %s{idx-%u}",
                        fsm->_name,
                        fsm->_table[i].event->_name,
                        fsm->_table[i].event->index,
                        fsm->_table[j].event->_name,
                        fsm->_table[j].event->index
                    );
                }
            }
            if (!fsm->_table[i].source || !fsm->_table[j].source ||
                !fsm->_table[i].target || !fsm->_table[j].target ||
                !fsm->_table[i].action || !fsm->_table[j].action
            ) {
                continue;
            }
            if (fsm->_table[i].source == fsm->_table[j].source &&
                fsm->_table[i].target == fsm->_table[j].target &&
                fsm->_table[i].action == fsm->_table[j].action
            ) {
                printTagLog(
                    FSM_GC_TAG,
                    "WARNING! \"%s\" has duplicate transitions [idx-%u == idx-%u] %s{0x%08X} -> %s{0x%08X} -> %s{0x%08X}",
                    fsm->_name,
                    i,
                    j,
                    fsm->_table[j].source->_name,
					(size_t)(size_t*)fsm->_table[j].source,
                    fsm->_table[j].target->_name,
					(size_t)(size_t*)fsm->_table[j].target,
                    fsm->_table[j].action ? fsm->_table[j].action->_name : EMPTY,
					(size_t)(size_t*)fsm->_table[j].action
                );
            }
        }
        if (fsm->_table[i].source && fsm->_table[i].event && fsm->_table[i].target) {
            continue;
        }
        fsm_gc_transition_t broken = fsm->_table[i];
        if (!broken.source) {
            printTagLog(FSM_GC_TAG, "\"%s\" transition idx-%u has empty source", fsm->_name, i);
        }
        if (!broken.event) {
            printTagLog(FSM_GC_TAG, "\"%s\" transition idx-%u has empty event", fsm->_name, i);
        }
        if (!broken.target) {
            printTagLog(FSM_GC_TAG, "\"%s\" transition idx-%u has empty target", fsm->_name, i);
        }
    }
#endif

    // Remove invalid transitions and set event index
    unsigned i = 0;
    while (i < fsm->_table_size) {
        if (fsm->_table[i].source && fsm->_table[i].event && fsm->_table[i].target) {
            if (!fsm->_table[i].event->index) {
                fsm->_table[i].event->index = ++_fsm_gc_events_iterator;
            }
            i++;
            continue;
        }
        for (unsigned j = i; j < fsm->_table_size - 1; j++) {
            fsm->_table[j] = fsm->_table[j+1];
        }
        fsm->_table_size--;
    }

    // Sort transitions by event priority
    for (unsigned i = 0; i < fsm->_table_size; i++) {
        for (unsigned j = i; j < fsm->_table_size - 1; j++) {
            if (fsm->_table[j].event->priority > fsm->_table[j + 1].event->priority) {
                continue;
            }
            fsm_gc_transition_t tmp = fsm->_table[j];
            fsm->_table[j] = fsm->_table[j + 1];
            fsm->_table[j + 1] = tmp;
        }
    }

#ifdef FSM_GC_BEDUG
    if (_log_enabled(fsm)) {
        if (fsm->_table_size) {
            printTagLog(
                FSM_GC_TAG, 
                "\"%s\" has been initialized with %u transitions",
                fsm->_name,
                fsm->_table_size
            );
        } else {
            printTagLog(
                FSM_GC_TAG, 
                "WARNING! \"%s\" has been initialized with empty transition table",
                fsm->_name
            );
        }
    }
#endif

    if (fsm->_table_size) {
        fsm->_initialized = true;
    }
}

void fsm_gc_disable_all_messages()
{
#ifdef FSM_GC_BEDUG
    disable_all_messages = true;
#endif
}

void fsm_gc_enable_all_messages()
{
#ifdef FSM_GC_BEDUG
    disable_all_messages = false;
#endif
}

void fsm_gc_reset(fsm_gc_t* fsm)
{
    if (!_check_initialized(fsm)) {
        return;
    }
    fsm->_events_cnt = 0;
    fsm->_state = fsm->_table[0].source;
}

void fsm_gc_process(fsm_gc_t* fsm) 
{
    if (!_check_initialized(fsm)) {
        return;
    }

    fsm->_state->state();

    bool is_transition = false;
    uint32_t table_idx = 0;
    size_t event_idx = 0;
    for (unsigned i = 0; i < fsm->_events_cnt; i++) {
        const fsm_gc_event_t* event = fsm->_events_queue[i];
        for (unsigned j = 0; j < fsm->_table_size; j++) {
            const fsm_gc_transition_t* tr = &fsm->_table[j];
            if (fsm->_state->state != tr->source->state) {
                continue;
            }
            if (event->index != tr->event->index) {
                continue;
            }
            is_transition = true;
            event_idx = i;
            table_idx = j;
            break;
        }
        if (is_transition) {
            break;
        }
    }

    if (!is_transition) {
        return;
    }

#ifdef FSM_GC_BEDUG
    if (_log_enabled(fsm)) {
		printTagLog(
			FSM_GC_TAG,
			"\"%s\" transition: %s{%s} -> %s -> %s",
			fsm->_name,
			fsm->_table[table_idx].source->_name,
			fsm->_table[table_idx].event->_name,
			fsm->_table[table_idx].action ? fsm->_table[table_idx].action->_name : EMPTY,
			fsm->_table[table_idx].target->_name
		);
    }
#endif

    fsm->_state = fsm->_table[table_idx].target;
    for (unsigned i = event_idx; i < fsm->_events_cnt - 1; i++) {
        fsm->_events_queue[i] = fsm->_events_queue[i + 1];
    }
    fsm->_events_cnt--;

    if (fsm->_table[table_idx].action) {
        fsm->_table[table_idx].action->action();
    }
}

void fsm_gc_push_event(fsm_gc_t* fsm, fsm_gc_event_t* event)
{
    if (!_check_initialized(fsm)) {
        return;
    }

    if (!event) {
#ifdef FSM_GC_BEDUG
        BEDUG_ASSERT(!fsm->_enable_msg || disable_all_messages, "Empty FSM event pointer");
#endif
        return;
    }
    
    if (fsm->_events_cnt >= FSM_GC_EVENTS_COUNT) {
        for (unsigned i = 0; i < FSM_GC_EVENTS_COUNT - 1; i++) {
            fsm->_events_queue[i] = fsm->_events_queue[i+1];
        }
        fsm->_events_cnt = FSM_GC_EVENTS_COUNT - 1;
    }
    fsm->_events_queue[fsm->_events_cnt++] = event;

    for (unsigned i = 0; i < fsm->_events_cnt; i++) {
        for (unsigned j = i; j < fsm->_events_cnt; j++) {
            if (fsm->_events_queue[i]->priority > fsm->_events_queue[j]->priority) {
                continue;
            }
            fsm_gc_event_t* tmp = fsm->_events_queue[i];
            fsm->_events_queue[i] = fsm->_events_queue[j];
            fsm->_events_queue[j] = tmp;
        }
    }

#ifdef FSM_GC_BEDUG
    if (_log_enabled(fsm)) {
		printTagLog(
			FSM_GC_TAG, \
			"\"%s\" push %02u event: %s",
			fsm->_name,
			fsm->_events_cnt - 1,
			event->_name
		);
    }
#endif
}

void fsm_gc_clear(fsm_gc_t* fsm)
{
    if (!_check_initialized(fsm)) {
        return;
    }

    fsm->_events_cnt = 0;

#ifdef FSM_GC_BEDUG
    if (_log_enabled(fsm)) {
		printTagLog(
			FSM_GC_TAG, \
			"\"%s\" clear",
			fsm->_name
		);
    }
#endif
}

bool fsm_gc_is_state(fsm_gc_t* fsm, fsm_gc_state_t* state)
{
    if (!_check_initialized(fsm)) {
        return false;
    }
    if (!state) {
#ifdef FSM_GC_BEDUG
        BEDUG_ASSERT(!_log_enabled(fsm), "Empty FSM state pointer");
#endif
        return false;
    }
    if (!fsm->_state) {
#ifdef FSM_GC_BEDUG
        BEDUG_ASSERT(!_log_enabled(fsm) || !fsm->_fsm_not_i, "FSM bad state");
        fsm->_fsm_not_i = true;
#endif
        return false;
    }
    return fsm->_state->state == state->state;
}

void fsm_gc_enable_messages(fsm_gc_t* fsm)
{
    if (!fsm) {
#ifdef FSM_GC_BEDUG
        BEDUG_ASSERT(disable_all_messages, "Empty FSM pointer");
#endif
        return;
    }
#ifdef FSM_GC_BEDUG
	fsm->_enable_msg = true;
#endif
}

void fsm_gc_disable_messages(fsm_gc_t* fsm)
{
    if (!fsm) {
#ifdef FSM_GC_BEDUG
        BEDUG_ASSERT(disable_all_messages, "Empty FSM pointer");
#endif
        return;
    }
#ifdef FSM_GC_BEDUG
	fsm->_enable_msg = false;
#endif
}

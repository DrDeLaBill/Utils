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

bool fsm_gc_init(fsm_gc_t* fsm, fsm_gc_transition_t* table, unsigned size)
{
    if (!fsm) {
#ifdef FSM_GC_BEDUG
        BEDUG_ASSERT(disable_all_messages, "Empty FSM pointer");
#endif
        return false;
    }

    fsm->_initialized = false;

    if (!size || !table) {
#ifdef FSM_GC_BEDUG
        BEDUG_ASSERT(!_log_enabled(fsm) || !fsm->_e_fsm_tt, "Empty FSM transition table");
        fsm->_e_fsm_tt = true;
#endif
        return false;
    }

    fsm->_table      = table;
    fsm->_table_size = size;
    fsm->_events_cnt = 0;
    

#ifdef FSM_GC_BEDUG
    // Check repeated and broken transitions
    for (unsigned i = 0; i < fsm->_table_size; i++) {
        if (!_log_enabled(fsm)) {
            continue;
        }
        bool tr1_err = false;
        fsm_gc_transition_t* tr1 = &fsm->_table[i];
        if (!tr1->source) {
            printTagLog(FSM_GC_TAG, "\"%s\" transition idx-%u has empty source", fsm->_name, i);
            tr1_err = true;
        } else if (!tr1->source->state_f) {
            printTagLog(FSM_GC_TAG, "\"%s\" transition idx-%u has empty state function", fsm->_name, i);
            tr1_err = true;
        }
        if (!tr1->event) {
            printTagLog(FSM_GC_TAG, "\"%s\" transition idx-%u has empty event", fsm->_name, i);
            tr1_err = true;
        }
        if (!tr1->target) {
            printTagLog(FSM_GC_TAG, "\"%s\" transition idx-%u has empty target", fsm->_name, i);
            tr1_err = true;
        } else if (!tr1->target->state_f) {
            printTagLog(FSM_GC_TAG, "\"%s\" transition idx-%u has empty state function", fsm->_name, i);
            tr1_err = true;
        }
        if (tr1_err) {
            continue;
        }
        for (unsigned j = i + 1; j < fsm->_table_size; j++) {
            fsm_gc_transition_t* tr2 = &fsm->_table[j];
            if (tr1->source && tr2->source) {
                if (tr1->source != tr2->source &&
                    tr1->source->state_f == tr2->source->state_f
                ) {
                    printTagLog(
                        FSM_GC_TAG, 
                        "WARNING! \"%s\" has matches functions states %s{0x%08X} = %s{0x%08X}",
                        fsm->_name,
                        tr1->source->_name,
                        (size_t)(size_t*)tr1->source,
                        tr2->source->_name,
                        (size_t)(size_t*)tr2->source
                    );
                }
            }
            if (tr1->target && tr2->target) {
                if (tr1->target != tr2->target &&
                    tr1->target->state_f == tr2->target->state_f
                ) {
                    printTagLog(
                        FSM_GC_TAG, 
                        "WARNING! \"%s\" has matches functions states %s{0x%08X} = %s{0x%08X}",
                        fsm->_name,
                        tr1->target->_name,
                        (size_t)(size_t*)tr1->target,
                        tr2->target->_name,
                        (size_t)(size_t*)tr2->target
                    );
                }
            }
            if (tr1->event && tr2->event &&
                tr1->source && tr2->source
            ) {
                if (tr1->event == tr2->event && 
                    tr1->source == tr2->source
                ) {
                    printTagLog(
                        FSM_GC_TAG, 
                        "WARNING! \"%s\" has matches functions events %s{0x%08X} = %s{0x%08X}",
                        fsm->_name,
                        tr1->event->_name,
                        (size_t)(size_t*)tr1->event,
                        tr2->event->_name,
                        (size_t)(size_t*)tr1->event
                    );
                }
            }
            if (!tr2->source || !tr2->target || !tr2->action) {
                continue;
            }
            if (tr1->source == tr2->source &&
                tr1->target == tr2->target &&
                tr1->action == tr2->action
            ) {
                printTagLog(
                    FSM_GC_TAG,
                    "WARNING! \"%s\" has duplicate transitions [idx-%u equals idx-%u] %s{0x%08X} -> %s{0x%08X} -> %s{0x%08X}",
                    fsm->_name,
                    i,
                    j,
                    tr2->source->_name,
					(size_t)(size_t*)tr2->source,
                    tr2->target->_name,
					(size_t)(size_t*)tr2->target,
                    tr2->action ? tr2->action->_name : EMPTY,
					(size_t)(size_t*)tr2->action
                );
            }
        }
    }
#endif

    // Remove invalid transitions and set event index
    for (unsigned i = 0; i < fsm->_table_size;) {
        if (fsm->_table[i].source && fsm->_table[i].event && fsm->_table[i].target) {
            if (fsm->_table[i].source->state_f && fsm->_table[i].target->state_f) {
                i++;
                continue;
            }
        }
        memmove(
            &fsm->_table[i],
            &fsm->_table[i+1],
            sizeof(fsm_gc_transition_t) * (fsm->_table_size - i - 1)
        );
        fsm->_table_size--;
    }
    if (fsm->_table_size) {
        fsm->_state = fsm->_table[0].source;
        fsm->_init_state = fsm->_table[0].source;
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

    return fsm->_initialized;
}

void fsm_gc_disable_all_messages(void)
{
#ifdef FSM_GC_BEDUG
    disable_all_messages = true;
#endif
}

void fsm_gc_enable_all_messages(void)
{
#ifdef FSM_GC_BEDUG
    disable_all_messages = false;
#endif
}

void fsm_gc_process(fsm_gc_t* fsm) 
{
    if (!_check_initialized(fsm)) {
        return;
    }

    fsm->_state->state_f();

    bool is_transition = false;
    uint32_t table_idx = 0;
    size_t event_idx = 0;
    for (unsigned i = 0; i < fsm->_events_cnt; i++) {
        const fsm_gc_event_t* event = fsm->_events_queue[i];
        for (unsigned j = 0; j < fsm->_table_size; j++) {
            const fsm_gc_transition_t* tr = &fsm->_table[j];
            if (fsm->_state != tr->source) {
                continue;
            }
            if (event != tr->event) {
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
    memmove(
        (void*)&fsm->_events_queue[event_idx],
        (void*)&fsm->_events_queue[event_idx + 1],
        sizeof(fsm_gc_event_t*) * (fsm->_events_cnt - event_idx - 1)
    );
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
        memmove(
            (void*)&fsm->_events_queue[0],
            (void*)&fsm->_events_queue[1],
            sizeof(fsm_gc_event_t*) * (FSM_GC_EVENTS_COUNT - 1)
        );
        fsm->_events_cnt = FSM_GC_EVENTS_COUNT - 1;
    }
    fsm->_events_queue[fsm->_events_cnt++] = event;

    for (unsigned i = 0; i < fsm->_events_cnt; i++) {
        for (unsigned j = i + 1; j < fsm->_events_cnt; j++) {
            if (fsm->_events_queue[i]->priority >= fsm->_events_queue[j]->priority) {
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

void fsm_gc_reset(fsm_gc_t* fsm)
{
    if (!_check_initialized(fsm)) {
        return;
    }
    fsm->_events_cnt = 0;
    fsm->_state = fsm->_init_state;
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
    return fsm->_state == state;
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

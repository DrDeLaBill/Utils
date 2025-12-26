/* Copyright © 2025 Georgy E. All rights reserved. */

#include "gstate.h"

#include <string.h>

#include "glog.h"
#include "bmacro.h"



#ifdef GSTATE_BEDUG

static const char GSTATE_TAG[] = "GSTE";
static bool disable_all_messages = false;

#endif


static bool _log_enabled(const gstate_t* gstate)
{
#ifdef GSTATE_BEDUG
    return gstate->_enable_msg && !disable_all_messages;
#else
    return false;
#endif
}

static bool _check_initialized(gstate_t* const gstate)
{
    if (!gstate) {
#ifdef GSTATE_BEDUG
        BEDUG_ASSERT(disable_all_messages, "Empty GSTATE pointer");
#endif
        return false;
    }
    if (!gstate->_initialized) {
#ifdef GSTATE_BEDUG
        BEDUG_ASSERT(!_log_enabled(gstate) || !gstate->_gstate_not_i, "GSTATE has not initialized");
        BEDUG_ASSERT(!_log_enabled(gstate) || !gstate->_gstate_not_i, "GSTATE state_f, table and event must not be NULL");
        gstate->_gstate_not_i = true;
#endif
        return false;
    }
    return true;
}

void gstate_init(gstate_t* gstate, gstate_state_t* states_list, size_t states_count)
{
    if (!gstate) {
#ifdef GSTATE_BEDUG
        BEDUG_ASSERT(disable_all_messages, "Empty GSTATE pointer");
#endif
        return;
    }
    gstate->_initialized = false;

    if (!states_count || !states_list) {
#ifdef GSTATE_BEDUG
        BEDUG_ASSERT(!_log_enabled(gstate) || !gstate->_e_gstate_list, "Empty GSTATE states list");
        gstate->_e_gstate_list = true;
#endif
        return;
    }

    if (!circle_buf_gc_init(gstate->_states_queue, (uint8_t*)gstate->_states_buf, sizeof(*gstate->_states_buf), GSTATE_BUFFER_COUNT)) {
#ifdef GSTATE_BEDUG
        BEDUG_ASSERT(!_log_enabled(gstate) || !gstate->_e_gstate_list, "States queue initialize error");
#endif
        return;
    }

    gstate->_states_list  = states_list;
    gstate->_states_count = states_count;
    gstate->_curr_state   = &gstate->_states_list[0];

    for (unsigned i = 0; i < gstate->_states_count; i++) {
        for (unsigned j = i + 1; j < gstate->_states_count; j++) {
            if (
                gstate->_states_list[i].state_f == gstate->_states_list[j].state_f ||
                !gstate->_states_list[j].state_f
            ) {
#ifdef GSTATE_BEDUG
                if (_log_enabled(gstate) && gstate->_states_list[j].state_f) {
                    printTagLog(
                        GSTATE_TAG, 
                        "WARNING! \"%s\" has matches functions states %s{0x%08X} = %s{0x%08X}",
                        gstate->_name,
                        gstate->_states_list[i]._name,
                        (size_t)(size_t*)gstate->_states_list[i].state_f,
                        gstate->_states_list[j]._name,
                        (size_t)(size_t*)gstate->_states_list[j].state_f
                    );
                } else if (_log_enabled(gstate)) {
                    printTagLog(
                        GSTATE_TAG, 
                        "WARNING! \"%s\" has empty function state at index %u",
                        gstate->_name,
                        j
                    );
                }
#endif
                memcpy(&gstate->_states_list[j], &gstate->_states_list[j + 1], (gstate->_states_count - j - 1) * sizeof(gstate_state_t));
                gstate->_states_count--;
            }
        }
    }

#ifdef GSTATE_BEDUG
    if (_log_enabled(gstate)) {
        printTagLog(GSTATE_TAG, "\"%s\" has been initialized", gstate->_name);
    }
#endif

    gstate->_initialized = true;
}

void gstate_disable_all_messages()
{
#ifdef GSTATE_BEDUG
    disable_all_messages = true;
#endif
}

void gstate_enable_all_messages()
{
#ifdef GSTATE_BEDUG
    disable_all_messages = false;
#endif
}

void gstate_reset(gstate_t* gstate)
{
    if (!_check_initialized(gstate)) {
        return;
    }
    circle_buf_gc_free(gstate->_states_queue);
    gstate->_curr_state = &gstate->_states_list[0];
}

void gstate_process(gstate_t* gstate) 
{
    if (!_check_initialized(gstate)) {
        return;
    }

    gstate->_curr_state->state_f();

    gstate_state_t* target = NULL;
    while (circle_buf_gc_count(gstate->_states_queue)) {
        const gstate_state_t* queue_item = (gstate_state_t*)circle_buf_gc_pop_back(gstate->_states_queue);
        if (!queue_item) {
            continue;
        }
        if (queue_item->state_f == gstate->_curr_state->state_f) {
            continue;
        }
        for (unsigned i = 0; i < gstate->_states_count; i++) {
            const gstate_state_t* item_state = &gstate->_states_list[i];
            if (!item_state) {
                continue;
            }                  
            if (!item_state->state_f) {
                continue;
            }
            if (item_state->state_f != queue_item->state_f) {
                continue;
            }
            if (!target) {
                target = (gstate_state_t*)queue_item;
                continue;
            }
            if (target->priority >= queue_item->priority) {
                target = (gstate_state_t*)queue_item;
                continue;
            }
        }
    }

    if (!target) {
        return;
    }

#ifdef GSTATE_BEDUG
    if (_log_enabled(gstate)) {
		printTagLog(
			GSTATE_TAG,
			"\"%s\" transition: %s -> %s",
			gstate->_name,
			gstate->_curr_state->_name,
            target->_name
		);
    }
#endif

    gstate->_curr_state = target;
}

void gstate_request_state(gstate_t* gstate, gstate_state_t* target)
{
    if (!_check_initialized(gstate)) {
        return;
    }

    if (!target) {
#ifdef GSTATE_BEDUG
        BEDUG_ASSERT(!gstate->_enable_msg || disable_all_messages, "Empty GSTATE state pointer");
#endif
        return;
    }
    
    circle_buf_gc_push_back(gstate->_states_queue, (uint8_t*)target);

#ifdef GSTATE_BEDUG
    if (_log_enabled(gstate)) {
		printTagLog(
			GSTATE_TAG, \
			"\"%s\" request %02u state: %s",
			gstate->_name,
			circle_buf_gc_count(gstate->_states_queue),
			target->_name
		);
    }
#endif
}

void gstate_clear(gstate_t* gstate)
{
    if (!_check_initialized(gstate)) {
        return;
    }

    circle_buf_gc_free(gstate->_states_queue);

#ifdef GSTATE_BEDUG
    if (_log_enabled(gstate)) {
		printTagLog(
			GSTATE_TAG, \
			"\"%s\" clear queue",
			gstate->_name
		);
    }
#endif
}

bool gstate_is_state(gstate_t* gstate, gstate_state_t* state)
{
    if (!_check_initialized(gstate)) {
        return false;
    }
    
    if (!state) {
#ifdef GSTATE_BEDUG
        BEDUG_ASSERT(!_log_enabled(gstate), "Empty GSTATE state_f pointer");
#endif
        return false;
    }
    
    if (!gstate->_curr_state) {
#ifdef GSTATE_BEDUG
        BEDUG_ASSERT(!_log_enabled(gstate) || !gstate->_gstate_not_i, "GSTATE bad state");
        gstate->_gstate_not_i = true;
#endif
        return false;
    }
    return gstate->_curr_state->state_f == state->state_f;
}

void gstate_enable_messages(gstate_t* gstate)
{
    if (!gstate) {
#ifdef GSTATE_BEDUG
        BEDUG_ASSERT(disable_all_messages, "Empty GSTATE pointer");
#endif
        return;
    }
#ifdef GSTATE_BEDUG
	gstate->_enable_msg = true;
#endif
}

void gstate_disable_messages(gstate_t* gstate)
{
    if (!gstate) {
#ifdef GSTATE_BEDUG
        BEDUG_ASSERT(disable_all_messages, "Empty GSTATE pointer");
#endif
        return;
    }
#ifdef GSTATE_BEDUG
	gstate->_enable_msg = false;
#endif
}

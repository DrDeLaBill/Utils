/* Copyright © 2024 Georgy E. All rights reserved. */

#ifndef _FSM_GC_H_
#define _FSM_GC_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>

#include "utils.h"


#define _FSM_GC_EVENTS_COUNT (10)


typedef struct _fsm_gc_event_t {
    size_t index;
} fsm_gc_event_t;

typedef struct _fsm_gc_state_t {
    void (*state) (void);
} fsm_gc_state_t;

typedef struct _fsm_gc_transition_t {
    fsm_gc_state_t* source;
    fsm_gc_event_t* event;
    fsm_gc_state_t* target;
} fsm_gc_transition_t;

typedef struct _fsm_gc_t {
    bool                 _initialized;
    fsm_gc_state_t*      _state;
    uint8_t              _events_count;
    fsm_gc_event_t       _events[_FSM_GC_EVENTS_COUNT];
    fsm_gc_transition_t* _table;
    size_t               _table_size;
} fsm_gc_t;


static size_t _fsm_gc_events_count = 1;


#define FSM_GC_CREATE_STATE(NAME, FUNC)       fsm_gc_state_t NAME = { FUNC };

#define FSM_GC_CREATE_EVENT(NAME)             fsm_gc_event_t NAME = { 0 };
                                              
#define FSM_GC_CREATE_TRANSITION(NAME, SOURCE, EVENT, TARGET) \
                                              fsm_gc_transition_t NAME { .source = SOURCE, .event = EVENT, .target = TARGET };
                                              
#define FSM_GC_CREATE_TABLE(NAME, ...)        fsm_gc_transition_t NAME[] = { __VA_ARGS__ };

#define FSM_GC_CREATE(FSM_NAME)               fsm_gc_t FSM_NAME = { \
                                                  ._initialized = false, \
                                                  ._state = NULL, \
                                                  ._events_count = 0, \
                                                  ._events = {0}, \
                                                  ._table = NULL, \
                                                  ._table_size = 0 \
                                              };


void fsm_gc_init(fsm_gc_t* fsm, fsm_gc_transition_t* table, unsigned size);
void fsm_gc_proccess(fsm_gc_t* fsm);
void fsm_gc_push_event(fsm_gc_t* fsm, fsm_gc_event_t* event);



#ifdef __cplusplus
}
#endif


#endif
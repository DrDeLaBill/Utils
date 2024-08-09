/* Copyright © 2024 Georgy E. All rights reserved. */

#ifndef _FSM_GC_H_
#define _FSM_GC_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>

#include "gutils.h"


#define _FSM_GC_EVENTS_COUNT (10)


typedef struct _fsm_gc_event_t {
    size_t index;
    size_t priority;
} fsm_gc_event_t;

typedef struct _fsm_gc_state_t {
    void (*state) (void);
} fsm_gc_state_t;

typedef struct _fsm_gc_transition_t {
    fsm_gc_state_t* source;
    fsm_gc_event_t* event;
    fsm_gc_state_t* target;
    void (*action) (void);
} fsm_gc_transition_t;

typedef struct _fsm_gc_t {
    bool                 _initialized;
    fsm_gc_state_t*      _state;
    uint8_t              _events_count;
    fsm_gc_event_t       _events[_FSM_GC_EVENTS_COUNT];
    fsm_gc_transition_t* _table;
    size_t               _table_size;
#ifdef DEBUG
    bool                 _e_fsm_tt;
    bool                 _fsm_not_i;
#endif
} fsm_gc_t;


#define FSM_GC_CREATE_STATE(NAME, FUNC)       fsm_gc_state_t NAME = { FUNC };

#define FSM_GC_CREATE_EVENT(NAME, PRIO)       fsm_gc_event_t NAME = { 0, PRIO };
                                              
#define FSM_GC_CREATE_TABLE(NAME, ...)        fsm_gc_transition_t NAME[] = { __VA_ARGS__ };

#ifdef DEBUG
#   define FSM_GC_CREATE(FSM_NAME)            fsm_gc_t FSM_NAME = { \
                                                  /* ._initialized = */  false, \
                                                  /* ._state = */        NULL, \
                                                  /* ._events_count = */ 0, \
                                                  /* ._events = */       {0}, \
                                                  /* ._table = */        NULL, \
                                                  /* ._table_size = */   0, \
	                                              /* _e_fsm_tt = */      false, \
	                                              /* _fsm_not_i = */     false \
                                              };
#else
#   define FSM_GC_CREATE(FSM_NAME)            fsm_gc_t FSM_NAME = { \
                                                  /* ._initialized = */  false, \
                                                  /* ._state = */        NULL, \
                                                  /* ._events_count = */ 0, \
                                                  /* ._events = */       {0}, \
                                                  /* ._table = */        NULL, \
                                                  /* ._table_size = */   0 \
                                              };
#endif


void fsm_gc_init(fsm_gc_t* fsm, fsm_gc_transition_t* table, unsigned size);
void fsm_gc_proccess(fsm_gc_t* fsm);
void fsm_gc_push_event(fsm_gc_t* fsm, fsm_gc_event_t* event);
void fsm_gc_clear(fsm_gc_t* fsm);
bool fsm_gc_is_state(fsm_gc_t* fsm, fsm_gc_state_t* state);



#ifdef __cplusplus
}
#endif


#endif

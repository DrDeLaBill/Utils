/* Copyright © 2024 Georgy E. All rights reserved. */

#ifndef _FSM_GC_H_
#define _FSM_GC_H_


#ifdef __cplusplus
extern "C" {
#endif


#ifndef FSM_GC_NO_DEBUG
#   ifdef DEBUG
#       define FSM_GC_BEDUG
#   endif
#endif


#include <stdint.h>
#include <stdbool.h>

#include "gutils.h"


#define _FSM_GC_EVENTS_COUNT (10)


#ifdef FSM_GC_BEDUG

#   ifndef FSM_GC_EVENT_NAME_SIZE
#      define FSM_GC_EVENT_NAME_SIZE  (32)
#   endif


#   ifndef FSM_GC_STATE_NAME_SIZE
#      define FSM_GC_STATE_NAME_SIZE  (32)
#   endif


#   ifndef FSM_GC_NAME_SIZE
#      define FSM_GC_NAME_SIZE        (32)
#   endif

#endif


typedef struct _fsm_gc_event_t {
    size_t index;
    size_t priority;
#ifdef FSM_GC_BEDUG
    char   _name[FSM_GC_EVENT_NAME_SIZE];
#endif
} fsm_gc_event_t;

typedef struct _fsm_gc_state_t {
    void (*state) (void);
#ifdef FSM_GC_BEDUG
    char _name[FSM_GC_STATE_NAME_SIZE];
#endif
} fsm_gc_state_t;

typedef struct _fsm_gc_action_t {
    void (*action) (void);
#ifdef FSM_GC_BEDUG
    char _name[FSM_GC_STATE_NAME_SIZE];
#endif
} fsm_gc_action_t;

typedef struct _fsm_gc_transition_t {
    fsm_gc_state_t*  source;
    fsm_gc_event_t*  event;
    fsm_gc_state_t*  target;
    fsm_gc_action_t* action;
} fsm_gc_transition_t;

typedef struct _fsm_gc_t {
    bool                 _initialized;
    fsm_gc_state_t*      _state;
    uint8_t              _events_count;
    fsm_gc_event_t       _events[_FSM_GC_EVENTS_COUNT];
    fsm_gc_transition_t* _table;
    size_t               _table_size;
#ifdef FSM_GC_BEDUG
    bool                 _e_fsm_tt;
    bool                 _fsm_not_i;
    char                 _name[FSM_GC_NAME_SIZE];
#endif
} fsm_gc_t;


extern size_t _fsm_gc_events_iterator;


#ifdef FSM_GC_BEDUG
#   define FSM_GC_CREATE_STATE(NAME, FUNC)      static void FUNC(void); \
                                                static fsm_gc_state_t NAME = { \
                                                    FUNC, \
                                                    __STR_DEF__(NAME) \
                                                };
#else
#   define FSM_GC_CREATE_STATE(NAME, FUNC)      static void FUNC(void); \
                                                static fsm_gc_state_t NAME = { \
                                                    FUNC \
                                                };
#endif

#ifdef FSM_GC_BEDUG
#   define FSM_GC_CREATE_EVENT(NAME, PRIO)      static fsm_gc_event_t NAME = { \
                                                    0, \
                                                    PRIO, \
                                                    __STR_DEF__(NAME) \
                                                };
#else
#   define FSM_GC_CREATE_EVENT(NAME, PRIO)      static fsm_gc_event_t NAME = { \
                                                    0, \
                                                    PRIO, \
                                                };
#endif

#ifdef FSM_GC_BEDUG
#   define FSM_GC_CREATE_ACTION(NAME, FUNC)     static void FUNC(void); \
                                                static fsm_gc_action_t NAME = { \
                                                    FUNC, \
                                                    __STR_DEF__(NAME) \
                                                };
#else
#   define FSM_GC_CREATE_ACTION(NAME, FUNC)     static void FUNC(void); \
                                                static fsm_gc_action_t NAME = { \
                                                    FUNC, \
                                                };
#endif
                                              
#define FSM_GC_CREATE_TABLE(NAME, ...)        static fsm_gc_transition_t NAME[] = { __VA_ARGS__ };

#ifdef FSM_GC_BEDUG
#   define FSM_GC_CREATE(FSM_NAME)            static fsm_gc_t FSM_NAME = { \
                                                  /* ._initialized = */  false, \
                                                  /* ._state = */        NULL, \
                                                  /* ._events_count = */ 0, \
                                                  /* ._events = */       {{0,0,""},}, \
                                                  /* ._table = */        NULL, \
                                                  /* ._table_size = */   0, \
                                                  /* _e_fsm_tt = */      false, \
                                                  /* _fsm_not_i = */     false, \
                                                  /* _name = */          __STR_DEF__(FSM_NAME) \
                                              };
#else
#   define FSM_GC_CREATE(FSM_NAME)            static fsm_gc_t FSM_NAME = { \
                                                  /* ._initialized = */  false, \
                                                  /* ._state = */        NULL, \
                                                  /* ._events_count = */ 0, \
                                                  /* ._events = */       {{0,0},}, \
                                                  /* ._table = */        NULL, \
                                                  /* ._table_size = */   0 \
                                              };
#endif


void fsm_gc_init(fsm_gc_t* fsm, fsm_gc_transition_t* table, unsigned size);
void fsm_gc_process(fsm_gc_t* fsm);
void fsm_gc_push_event(fsm_gc_t* fsm, fsm_gc_event_t* event);
void fsm_gc_clear(fsm_gc_t* fsm);
bool fsm_gc_is_state(fsm_gc_t* fsm, fsm_gc_state_t* state);

#ifdef FSM_GC_BEDUG
unsigned _fsm_gc_get_state_debug_number(const char* name);
unsigned _fsm_gc_get_event_debug_number(const char* name);
#endif



#ifdef __cplusplus
}
#endif


#endif

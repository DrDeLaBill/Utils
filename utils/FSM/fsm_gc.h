/* Copyright © 2025 Georgy E. All rights reserved. */

#ifndef _FSM_GC_H_
#define _FSM_GC_H_


#ifdef __cplusplus
extern "C" {
#endif


#ifndef FSM_GC_EVENTS_COUNT
#   define FSM_GC_EVENTS_COUNT (10)
#endif


#ifndef FSM_GC_NO_DEBUG
#   if defined(_DEBUG) || defined(DEBUG) || defined(GBEDUG_FORCE)
#       define FSM_GC_BEDUG
#   endif
#endif


#include <stdint.h>
#include <stdbool.h>

#include "gutils.h"
#include "circle_buf_gc.h"


typedef struct _fsm_gc_event_t {
    size_t      index;
    size_t      priority;
#ifdef FSM_GC_BEDUG
    const char* _name;
#endif
} fsm_gc_event_t;

typedef struct _fsm_gc_state_t {
    void        (*state) (void);
#ifdef FSM_GC_BEDUG
    const char* _name;
#endif
} fsm_gc_state_t;

typedef struct _fsm_gc_action_t {
    void        (*action) (void);
#ifdef FSM_GC_BEDUG
    const char* _name;
#endif
} fsm_gc_action_t;

typedef struct _fsm_gc_transition_t {
    fsm_gc_state_t*  const source;
    fsm_gc_event_t*  const event;
    fsm_gc_state_t*  const target;
    fsm_gc_action_t* const action;
} fsm_gc_transition_t;

typedef struct _fsm_gc_t {
    bool                 _initialized;
    fsm_gc_state_t*      _state;
    uint8_t              _events_length;
    fsm_gc_event_t*      _events_buf;
    circle_buf_gc_t*     _events;
    fsm_gc_transition_t* _table;
    size_t               _table_size;
#ifdef FSM_GC_BEDUG
    bool                 _e_fsm_tt;
    bool                 _fsm_not_i;
    const char*          _name;
    bool                 _enable_msg;
#endif
} fsm_gc_t;


#ifdef FSM_GC_BEDUG
#   define FSM_GC_CREATE_STATE(NAME, FUNC)      static const char __concat(__bedug_state_name, NAME)[] = __STR_DEF__(NAME); \
                                                static void FUNC(void); \
                                                static fsm_gc_state_t NAME = { \
                                                    FUNC, \
                                                    __concat(__bedug_state_name, NAME) \
                                                };
#else
#   define FSM_GC_CREATE_STATE(NAME, FUNC)      static void FUNC(void); \
                                                static fsm_gc_state_t NAME = { \
                                                    FUNC \
                                                };
#endif

#ifdef FSM_GC_BEDUG
#   define FSM_GC_CREATE_EVENT(NAME, PRIO)      static const char __concat(__bedug_event_name, NAME)[] = __STR_DEF__(NAME); \
                                                static fsm_gc_event_t NAME = { \
                                                    0, \
                                                    PRIO, \
                                                    __concat(__bedug_event_name, NAME) \
                                                };
#else
#   define FSM_GC_CREATE_EVENT(NAME, PRIO)      static fsm_gc_event_t NAME = { \
                                                    0, \
                                                    PRIO, \
                                                };
#endif

#ifdef FSM_GC_BEDUG
#   define FSM_GC_CREATE_ACTION(NAME, FUNC)     static const char __concat(__bedug_action_name, NAME)[] = __STR_DEF__(NAME); \
                                                static void FUNC(void); \
                                                static fsm_gc_action_t NAME = { \
                                                    FUNC, \
                                                    __concat(__bedug_action_name, NAME) \
                                                };
#else
#   define FSM_GC_CREATE_ACTION(NAME, FUNC)     static void FUNC(void); \
                                                static fsm_gc_action_t NAME = { \
                                                    FUNC, \
                                                };
#endif
                                              
#define FSM_GC_CREATE_TABLE(NAME, ...)        static fsm_gc_transition_t NAME[] = { __VA_ARGS__ };

#ifdef FSM_GC_BEDUG
#   define FSM_GC_CREATE(FSM_NAME)            static const char __concat(__bedug_fsm_name_, FSM_NAME)[] = __STR_DEF__(FSM_NAME); \
                                              static fsm_gc_event_t  __concat(__events_buf_, FSM_NAME)[FSM_GC_EVENTS_COUNT] = {{0,0,""}}; \
                                              static circle_buf_gc_t __concat(__events_, FSM_NAME) = { 0, NULL, 0, 0, 0, 0 };\
                                              static fsm_gc_t FSM_NAME = { \
                                                  /* ._initialized = */   false, \
                                                  /* ._state = */         NULL, \
                                                  /* ._events_length = */ __arr_len(__concat(__events_buf_, FSM_NAME)), \
                                                  /* ._events_buf = */    __concat(__events_buf_, FSM_NAME), \
                                                  /* ._events = */        &__concat(__events_, FSM_NAME), \
                                                  /* ._table = */         NULL, \
                                                  /* ._table_size = */    0, \
                                                  /* _e_fsm_tt = */       false, \
                                                  /* _fsm_not_i = */      false, \
                                                  /* _name = */           __concat(__bedug_fsm_name_, FSM_NAME), \
                                                  /* _enable_msg = */     true \
                                              };
#else
#   define FSM_GC_CREATE(FSM_NAME)            static fsm_gc_event_t  __concat(__events_buf_, FSM_NAME)[FSM_GC_EVENTS_COUNT] = {{0,0}}; \
                                              static circle_buf_gc_t __concat(__events_, FSM_NAME) = { 0, NULL, 0, 0, 0, 0 };\
                                              static fsm_gc_t FSM_NAME = { \
                                                  /* ._initialized = */   false, \
                                                  /* ._state = */         NULL, \
                                                  /* ._events_length = */ __arr_len(__concat(__events_buf_, FSM_NAME)), \
                                                  /* ._events_buf = */    __concat(__events_buf_, FSM_NAME), \
                                                  /* ._events = */        &__concat(__events_, FSM_NAME), \
                                                  /* ._table = */         NULL, \
                                                  /* ._table_size = */    0 \
                                              };
#endif


void fsm_gc_init(fsm_gc_t* fsm, fsm_gc_transition_t* table, unsigned size);
void fsm_gc_disable_all_messages();
void fsm_gc_enable_all_messages();
void fsm_gc_reset(fsm_gc_t* fsm);
void fsm_gc_process(fsm_gc_t* fsm);
void fsm_gc_push_event(fsm_gc_t* fsm, fsm_gc_event_t* event);
void fsm_gc_clear(fsm_gc_t* fsm);
bool fsm_gc_is_state(fsm_gc_t* fsm, fsm_gc_state_t* state);
void fsm_gc_enable_messages(fsm_gc_t* fsm);
void fsm_gc_disable_messages(fsm_gc_t* fsm);


#ifdef __cplusplus
}
#endif


#endif

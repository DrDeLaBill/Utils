/* Copyright © 2025 Georgy E. All rights reserved. */

#ifndef _G_STATE_H_
#define _G_STATE_H_


#ifdef __cplusplus
extern "C" {
#endif


#ifndef GSTATE_BUFFER_COUNT
    #define GSTATE_BUFFER_COUNT (5)
#endif


#ifndef GSTATE_NO_DEBUG
    #if defined(_DEBUG) || defined(DEBUG) || defined(GBEDUG_FORCE)
        #define GSTATE_BEDUG
    #endif
#endif


#include <stdint.h>
#include <stdbool.h>

#include "gutils.h"
#include "circle_buf_gc.h"


typedef struct _gstate_state_t {
    void        (*state_f) (void);
    uint8_t     priority;
#ifdef GSTATE_BEDUG
    const char* _name;
#endif
} gstate_state_t;


typedef struct _gstate_t {
    bool                 _initialized;
    gstate_state_t*      _curr_state;
    size_t               _states_count;
    gstate_state_t*      _states_list;
    gstate_state_t*      _states_buf;
    circle_buf_gc_t*     _states_queue;
#ifdef GSTATE_BEDUG
    bool                 _e_gstate_list;
    bool                 _gstate_not_i;
    const char*          _name;
    bool                 _enable_msg;
#endif
} gstate_t;


#ifdef GSTATE_BEDUG
    #define GSTATE_CREATE_STATE(NAME, FUNC, PRIO)   static const char __concat(__bedug_state_name, FUNC)[] = __STR_DEF__(FUNC); \
                                                    static void FUNC(void); \
                                                    static gstate_state_t NAME = { \
                                                        FUNC, \
                                                        PRIO, \
                                                        __concat(__bedug_state_name, FUNC) \
                                                    };
#else    
    #define GSTATE_CREATE_STATE(NAME, FUNC, PRIO)   static void FUNC(void); \
                                                    static gstate_state_t NAME = { \
                                                        FUNC, \
                                                        PRIO \
                                                    };
#endif

#define GSTATE_CREATE_LIST(NAME, ...)               static gstate_state_t NAME[] = { __VA_ARGS__ };

#ifdef GSTATE_BEDUG
    #define GSTATE_CREATE(GSTATE_NAME)              static const char __concat(__bedug_gstate_name_, GSTATE_NAME)[] = __STR_DEF__(GSTATE_NAME); \
                                                    static gstate_state_t  __concat(__states_queue_buf_, GSTATE_NAME)[GSTATE_BUFFER_COUNT] = {{NULL,0,""}}; \
                                                    static circle_buf_gc_t __concat(__states_queue_, GSTATE_NAME) = { 0, NULL, 0, 0, 0, 0 };\
                                                    static gstate_t GSTATE_NAME = { \
                                                        /* ._initialized = */  false, \
                                                        /* ._curr_state = */   NULL, \
                                                        /* ._states_count = */ 0, \
                                                        /* ._states_list = */  NULL, \
                                                        /* ._states_buf = */   __concat(__states_queue_buf_, GSTATE_NAME), \
                                                        /* ._states_queue = */ &__concat(__states_queue_, GSTATE_NAME), \
                                                        /* _e_gstate_list = */ false, \
                                                        /* _gstate_not_i = */  false, \
                                                        /* _name = */          __concat(__bedug_gstate_name_, GSTATE_NAME), \
                                                        /* _enable_msg = */    true \
                                                    };
#else
    #define GSTATE_CREATE(GSTATE_NAME)              static gstate_state_t  __concat(__states_queue_buf_, GSTATE_NAME)[GSTATE_BUFFER_COUNT] = {{0,0}}; \
                                                    static circle_buf_gc_t __concat(__states_queue_, GSTATE_NAME) = { 0, NULL, 0, 0, 0, 0 };\
                                                    static gstate_t GSTATE_NAME = { \
                                                        /* ._initialized = */  false, \
                                                        /* ._curr_state = */   NULL, \
                                                        /* ._states_count = */ 0, \
                                                        /* ._states_list = */  NULL, \
                                                        /* ._states_buf = */   __concat(__states_queue_buf_, GSTATE_NAME), \
                                                        /* ._states_queue = */ &__concat(__states_queue_, GSTATE_NAME), \
                                                    };
#endif


void gstate_init(gstate_t* gstate, gstate_state_t* states_list, size_t states_count);
void gstate_disable_all_messages();
void gstate_enable_all_messages();
void gstate_reset(gstate_t* gstate);
void gstate_process(gstate_t* gstate);
void gstate_request_state(gstate_t* gstate, gstate_state_t* target);
void gstate_clear(gstate_t* gstate);
bool gstate_is_state(gstate_t* gstate, gstate_state_t* state_f);
void gstate_enable_messages(gstate_t* gstate);
void gstate_disable_messages(gstate_t* gstate);


#ifdef __cplusplus
}
#endif


#endif

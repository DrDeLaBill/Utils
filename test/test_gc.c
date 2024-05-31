/* Copyright © 2024 Georgy E. All rights reserved. */

#include "test_gc.h"

#include <stdio.h>

#include "fsm_gc.h"


void func1(void) {
	printf("func1\n");
}

void func2(void) {
	printf("func2\n");
}

void func3(void) {
	printf("func3\n");
}


FSM_GC_CREATE(fsm);

FSM_GC_CREATE_STATE(state1, func1);
FSM_GC_CREATE_STATE(state2, func2);
FSM_GC_CREATE_STATE(state3, func3);

FSM_GC_CREATE_EVENT(event1);
FSM_GC_CREATE_EVENT(event2);

FSM_GC_CREATE_TABLE(
    fsm_table,
    { &state1, &event1, &state2 },
    { &state1, &event2, &state3 },
    { &state2, &event2, &state1 },
    { &state2, &event1, &state3 },
    { &state3, &event1, &state1 },
    { &state3, &event2, &state1 }
);


void utils_test_gc()
{
    fsm_gc_init(&fsm, fsm_table, __arr_len(fsm_table));
    
    for (unsigned i = 0; i < 10; i++) {
        fsm_gc_proccess(&fsm);
    }
    
    
    printf("push event1\n");
    fsm_gc_push_event(&fsm, &event1);
    printf("push event1\n");
    fsm_gc_push_event(&fsm, &event1);
    printf("push event1\n");
    fsm_gc_push_event(&fsm, &event1);


    for (unsigned i = 0; i < 10; i++) {
        fsm_gc_proccess(&fsm);
    }


    printf("push event2\n");
    fsm_gc_push_event(&fsm, &event2);
    printf("push event1\n");
    fsm_gc_push_event(&fsm, &event1);
    printf("push event2\n");
    fsm_gc_push_event(&fsm, &event2);
    printf("push event1\n");
    fsm_gc_push_event(&fsm, &event1);


    for (unsigned i = 0; i < 10; i++) {
        fsm_gc_proccess(&fsm);
    }
}
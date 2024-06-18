#ifdef UTILS_TEST

#include <iostream>

#include "glog.h"
#include "fsm_gc.h"
#include "CodeStopwatch.h"

#include "FiniteStateMachine.h"



static const char TAG[] = "TEST";

struct green_toggle {

	void operator()(void) const
	{
		printTagLog(TAG, "green_toggle");
	}
};

struct yellow_toggle {

	void operator()(void) const
	{
		printTagLog(TAG, "yellow_toggle");
	}
};

struct red_toggle {

	void operator()(void) const
	{
		printTagLog(TAG, "red_toggle");
	}
};

struct green_on {
	void operator()(void) const
	{
		printTagLog(TAG, "green_on");
	}
};

struct yellow_on {

	void operator()(void) const
	{
		printTagLog(TAG, "yellow_on");
	}
};

struct red_on {

	void operator()(void) const
	{
		printTagLog(TAG, "red_on");
	}
};

FSM_CREATE_STATE(green_s,  green_toggle);
FSM_CREATE_STATE(yellow_s, yellow_toggle);
FSM_CREATE_STATE(red_s,    red_toggle);

FSM_CREATE_EVENT(green_e, 0);
FSM_CREATE_EVENT(yellow_e, 0);
FSM_CREATE_EVENT(red_e, 0);
FSM_CREATE_EVENT(priority_e, 1);

using fsm_table = fsm::TransitionTable<
	fsm::Transition<green_s,  yellow_e,   yellow_s, yellow_on, fsm::Guard::NO_GUARD>,
	fsm::Transition<green_s,  priority_e, red_s,    red_on,    fsm::Guard::NO_GUARD>,
	fsm::Transition<yellow_s, red_e,      red_s,    red_on,    fsm::Guard::NO_GUARD>,
	fsm::Transition<red_s,    green_e,    green_s,  green_on,  fsm::Guard::NO_GUARD>
>;

void _gc_state1(void) {
	printTagLog(TAG, "gc_state1");
}

void _gc_state2(void) {
	printTagLog(TAG, "gc_state2");
}

void _gc_state3(void) {
	printTagLog(TAG, "gc_state3");
}


FSM_GC_CREATE(gc_fsm);

FSM_GC_CREATE_STATE(gc_state1, _gc_state1);
FSM_GC_CREATE_STATE(gc_state2, _gc_state2);
FSM_GC_CREATE_STATE(gc_state3, _gc_state3);

FSM_GC_CREATE_EVENT(gc_event1);
FSM_GC_CREATE_EVENT(gc_event2);

FSM_GC_CREATE_TABLE(
    gc_fsm_table,
    { &gc_state1, &gc_event1, &gc_state2 },
    { &gc_state1, &gc_event2, &gc_state3 },
    { &gc_state2, &gc_event2, &gc_state1 },
    { &gc_state2, &gc_event1, &gc_state3 },
    { &gc_state3, &gc_event1, &gc_state1 },
    { &gc_state3, &gc_event2, &gc_state1 }
);



int main()
{
	utl::CodeStopwatch stopwatch(TAG);

	printTagLog(TAG, "C++ Test");

	fsm::FiniteStateMachine<fsm_table> fsm;

	fsm.push_events(
		red_e{},
		yellow_e{},
		green_e{}
	);
	fsm.push_event(priority_e{});
	fsm.push_event(yellow_e{});
	fsm.push_event(green_e{});

	for (int i = 0; i < 10; i++) {
		fsm.proccess();
	}

	printTagLog(TAG, "OK\n");

	printTagLog(TAG, "C Test");

	fsm_gc_init(&gc_fsm, gc_fsm_table, __arr_len(gc_fsm_table));
    
    for (unsigned i = 0; i < 10; i++) {
        fsm_gc_proccess(&gc_fsm);
    }
    
    
    printTagLog(TAG, "push event1");
    fsm_gc_push_event(&gc_fsm, &gc_event1);
    printTagLog(TAG, "push event1");
    fsm_gc_push_event(&gc_fsm, &gc_event1);
    printTagLog(TAG, "push event1");
    fsm_gc_push_event(&gc_fsm, &gc_event1);


    for (unsigned i = 0; i < 6; i++) {
        fsm_gc_proccess(&gc_fsm);
    }


    printTagLog(TAG, "push event2");
    fsm_gc_push_event(&gc_fsm, &gc_event2);
    printTagLog(TAG, "push event1");
    fsm_gc_push_event(&gc_fsm, &gc_event1);
    printTagLog(TAG, "push event2");
    fsm_gc_push_event(&gc_fsm, &gc_event2);
    printTagLog(TAG, "push event1");
    fsm_gc_push_event(&gc_fsm, &gc_event1);


    for (unsigned i = 0; i < 6; i++) {
        fsm_gc_proccess(&gc_fsm);
    }

    return 0;
}

#endif
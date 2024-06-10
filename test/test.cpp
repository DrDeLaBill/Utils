#ifdef UTILS_TEST

#include <iostream>

#include "test_gc.h"

#include "log.h"
#include "CodeStopwatch.h"

#include "FiniteStateMachine.h"


struct green_toggle {

	void operator()(void) const
	{
		printTagLog("MAIN", "green_toggle");
	}
};

struct yellow_toggle {

	void operator()(void) const
	{
		printTagLog("MAIN", "yellow_toggle");
	}
};

struct red_toggle {

	void operator()(void) const
	{
		printTagLog("MAIN", "red_toggle");
	}
};

struct green_on {
	void operator()(void) const
	{
		printTagLog("MAIN", "green_on");
	}
};

struct yellow_on {

	void operator()(void) const
	{
		printTagLog("MAIN", "yellow_on");
	}
};

struct red_on {

	void operator()(void) const
	{
		printTagLog("MAIN", "red_on");
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


int main()
{
	utl::CodeStopwatch stopwatch("MAIN");

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

	printTagLog("MAIN", "OK");
	printTagLog("LOG", "OK");

	utils_test_gc();

    return 0;
}

#endif
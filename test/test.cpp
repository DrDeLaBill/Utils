#include <iostream>


#include "Log.h"
#include "CodeStopwatch.h"

#include "FiniteStateMachine.h"


struct green_toggle {

	void operator()(void) const
	{
		std::cout << "green_toggle" << std::endl;
	}
};

struct yellow_toggle {

	void operator()(void) const
	{
		std::cout << "yellow_toggle" << std::endl;
	}
};

struct red_toggle {

	void operator()(void) const
	{
		std::cout << "red_toggle" << std::endl;
	}
};

struct green_on {
	void operator()(void) const
	{
		std::cout << "green_on" << std::endl;
	}
};

struct yellow_on {

	void operator()(void) const
	{
		std::cout << "yellow_on" << std::endl;
	}
};

struct red_on {

	void operator()(void) const
	{
		std::cout << "red_on" << std::endl;
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
	// fsm.push_event(yellow_e{});
	// fsm.push_event(green_e{});

	for (int i = 0; i < 10; i++) {
		fsm.proccess();
	}

    return 0;
}
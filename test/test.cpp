#include <iostream>


#include "Log.h"
#include "CodeStopwatch.h"

#include "FSMKey.h"
#include "FSMGuard.h"
#include "FSMEvent.h"
#include "FSMState.h"
#include "FSMAction.h"
#include "FSMCharHash.h"
#include "FSMTransition.h"
#include "FSMTransitionTable.h"
#include "FiniteStateMachine.h"


#define STATE(str)	      str = fsm::State<fsm::charHash(#str)>;
#define STATE_A(str, act) str = fsm::State<fsm::charHash(#str), act>;
#define EVENT(str)	      str = fsm::Event<fsm::charHash(#str)>;


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


int main()
{
	utl::CodeStopwatch stopwatch("MAIN"); // TODO: fix cmake (INTERFACE library)

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

	using STATE_A(green_s, green_toggle);
	std::cout << "state green_s : index = " << green_s::index << std::endl;
	using STATE_A(yellow_s, yellow_toggle);
	std::cout << "state yellow_s : index = " << yellow_s::index << std::endl;
	using STATE_A(red_s, red_toggle);
	std::cout << "state red_s : index = " << red_s::index << std::endl;

	using EVENT(green_e);
	std::cout << "event green_e : index = " << green_e::index << std::endl;
	using EVENT(yellow_e);
	std::cout << "event yellow_e : index = " << yellow_e::index << std::endl;
	using EVENT(red_e);
	std::cout << "event red_e : index = " << red_e::index << std::endl;


	using a = fsm::Transition<green_s, yellow_e, yellow_s, yellow_on, fsm::Guard::NO_GUARD>;
	using b = fsm::Transition<yellow_s, red_e, red_s, red_on, fsm::Guard::NO_GUARD>;
	using c = fsm::Transition<red_s, green_e, green_s, green_on, fsm::Guard::NO_GUARD>;

	using fsm_table = fsm::TransitionTable<a, b, c>;

	fsm::FiniteStateMachine<fsm_table> fsm;

	fsm.push_event(red_e{});
	fsm.push_event(yellow_e{});
	fsm.push_event(green_e{});

	for (int i = 0; i < 10; i++) {
		fsm.proccess();
	}

    std::cout << "OK!" << std::endl;

    return 0;
}
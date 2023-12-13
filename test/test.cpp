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


#define STATE(str)	str = fsm::State<fsm::charHash(#str)>
#define STATE_A(str, act)	str = fsm::State<fsm::charHash(#str), act>
#define EVENT(str)	str = fsm::Event<fsm::charHash(#str)>


struct green_toggle {

	void operator()(void) const
	{
		std::cout << "green_toggle" << std::endl;
		// for (uint32_t i = 0; i != 10; i++) {
			// HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
			// HAL_Delay(150);
		// }
	}
};

struct yellow_toggle {

	void operator()(void) const
	{
		std::cout << "yellow_toggle" << std::endl;
		// for (uint32_t i = 0; i != 10; i++) {
			// HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
			// HAL_Delay(150);
		// }
	}
};

struct red_toggle {

	void operator()(void) const
	{
		std::cout << "red_toggle" << std::endl;
		// for (uint32_t i = 0; i != 10; i++) {
			// HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
			// HAL_Delay(150);
		// }
	}
};


int main()
{
	struct green_on {
		void operator()(void) const
		{
			std::cout << "green_on" << std::endl;
			// for (uint32_t i = 0; i != 10; i++) {
				// HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
				// HAL_Delay(50);
			// }
		}
	};

	struct yellow_on {

		void operator()(void) const
		{
			std::cout << "yellow_on" << std::endl;
			// for (uint32_t i = 0; i != 10; i++) {
				// HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
				// HAL_Delay(50);
			// }
		}
	};

	struct red_on {

		void operator()(void) const
		{
			std::cout << "red_on" << std::endl;
			// for (uint32_t i = 0; i != 10; i++) {
				// HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
				// HAL_Delay(50);
			// }
		}
	};

	using STATE_A(green_s, green_toggle);
	using STATE_A(yellow_s, yellow_toggle);
	using STATE_A(red_s, red_toggle);

	using EVENT(green_e);
	using EVENT(yellow_e);
	using EVENT(red_e);


	using a = fsm::Transition<green_s, yellow_e, yellow_s, yellow_on, fsm::Guard::NO_GUARD>;
	using b = fsm::Transition<yellow_s, red_e, red_s, red_on, fsm::Guard::NO_GUARD>;
	using c = fsm::Transition<red_s, green_e, green_s, green_on, fsm::Guard::NO_GUARD>;

	using fsm_table = fsm::TransitionTable<a, b, c>;

	fsm::FiniteStateMachine<fsm_table> fsm;

	fsm.push_event(red_e{});
	fsm.push_event(yellow_e{});
	fsm.push_event(green_e{});

	for (int i = 0; i < 1000; i++) {
		fsm.proccess();
	}

    std::cout << "OK!" << std::endl;

    return 0;
}
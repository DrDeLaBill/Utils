/* Copyright © 2025 Georgy E. All rights reserved. */

#include <gtest/gtest.h>

#include "glog.h"

#include "FiniteStateMachine.h"


static const char TAG[] = "FSM";


struct _green_s {
    void operator()(void) const
    {
        printTagLog(TAG, "green_s");
    }
};

struct _yellow_s {
    void operator()(void) const
    {
        printTagLog(TAG, "yellow_s");
    }
};

struct _red_s {
    void operator()(void) const
    {
        printTagLog(TAG, "red_s");
    }
};

struct green_a {
    void operator()(void) const
    {
        printTagLog(TAG, "green_a");
    }
};

struct yellow_a {
    void operator()(void) const
    {
        printTagLog(TAG, "yellow_a");
    }
};

struct red_a {
    void operator()(void) const
    {
        printTagLog(TAG, "red_a");
    }
};
  
FSM_CREATE_STATE(green_s,  _green_s);
FSM_CREATE_STATE(yellow_s, _yellow_s);
FSM_CREATE_STATE(red_s,    _red_s);

FSM_CREATE_EVENT(green_e,    0);
FSM_CREATE_EVENT(yellow_e,   0);
FSM_CREATE_EVENT(red_e,      0);
FSM_CREATE_EVENT(priority_e, 1);
  
using fsm_table = fsm::TransitionTable<
    fsm::Transition<green_s,  yellow_e,   yellow_s, yellow_a, fsm::Guard::NO_GUARD>,
    fsm::Transition<green_s,  priority_e, red_s,    red_a,    fsm::Guard::NO_GUARD>,
    fsm::Transition<yellow_s, red_e,      red_s,    red_a,    fsm::Guard::NO_GUARD>,
    fsm::Transition<yellow_s, priority_e, green_s,  green_a,  fsm::Guard::NO_GUARD>,
    fsm::Transition<red_s,    green_e,    green_s,  green_a,  fsm::Guard::NO_GUARD>
>;

GTEST_TEST(FiniteStateMachineSuite, TestGreenToYellow) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.push_event(yellow_e{});
    fsm.proccess();
    // Verify transition from green to yellow
    ASSERT_TRUE(fsm.is_state(yellow_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestGreenToRed) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.push_event(priority_e{});
    fsm.proccess();
    // Verify priority transition from green to red
    ASSERT_TRUE(fsm.is_state(red_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestYellowToRed) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.push_event(yellow_e{});
    fsm.proccess();
    fsm.push_event(red_e{});
    fsm.proccess();
    // Verify transition from yellow to red
    ASSERT_TRUE(fsm.is_state(red_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestRedToGreen) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.push_event(priority_e{});
    fsm.proccess();
    fsm.push_event(green_e{});
    fsm.proccess();
    // Verify transition from red to green
    ASSERT_TRUE(fsm.is_state(green_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestEventPriority) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.push_events(yellow_e{}, priority_e{});
    fsm.proccess();
    // Verify priority event takes precedence
    ASSERT_TRUE(fsm.is_state(red_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestMultipleTransitions) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.push_events(yellow_e{}, red_e{}, green_e{});
    for(int i = 0; i < 3; i++) fsm.proccess();
    ASSERT_TRUE(fsm.is_state(green_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestEmptyEventQueue) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.proccess();
    ASSERT_TRUE(fsm.is_state(green_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestPriorityInterrupt) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.push_events(yellow_e{}, priority_e{});
    fsm.proccess();
    ASSERT_TRUE(fsm.is_state(red_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestFullCycle) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.push_events(yellow_e{}, red_e{}, green_e{});
    for(int i = 0; i < 3; i++) fsm.proccess();
    ASSERT_TRUE(fsm.is_state(green_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestRedToGreenToYellow) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.push_events(priority_e{}, green_e{}, yellow_e{});
    for(int i = 0; i < 3; i++) fsm.proccess();
    ASSERT_TRUE(fsm.is_state(yellow_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestMultiplePriorityEvents) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.push_events(priority_e{}, priority_e{}, priority_e{});
    fsm.proccess();
    ASSERT_TRUE(fsm.is_state(red_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestYellowToRedToGreen) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.push_events(yellow_e{}, red_e{}, green_e{});
    for(int i = 0; i < 3; i++) fsm.proccess();
    ASSERT_TRUE(fsm.is_state(green_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestPriorityAfterNormalTransition) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.push_event(yellow_e{});
    fsm.proccess();
    fsm.push_event(priority_e{});
    fsm.proccess();
    ASSERT_TRUE(fsm.is_state(green_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestGreenToYellowToRed) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.push_events(yellow_e{}, red_e{});
    for(int i = 0; i < 2; i++) fsm.proccess();
    ASSERT_TRUE(fsm.is_state(red_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestInitialStateTransition) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    ASSERT_TRUE(fsm.is_state(green_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestMultipleProcessCalls) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.push_event(yellow_e{});
    for(int i = 0; i < 5; i++) fsm.proccess();
    ASSERT_TRUE(fsm.is_state(yellow_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestPriorityEventOverride) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.push_events(yellow_e{}, red_e{}, priority_e{});
    fsm.proccess();
    ASSERT_TRUE(fsm.is_state(red_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestCompleteCircuit) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.push_events(yellow_e{}, red_e{}, green_e{}, yellow_e{});
    for(int i = 0; i < 4; i++) fsm.proccess();
    ASSERT_TRUE(fsm.is_state(yellow_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestRedStatePriority) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.push_events(priority_e{}, yellow_e{});
    fsm.proccess();
    ASSERT_TRUE(fsm.is_state(red_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestStateSequence) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.push_events(yellow_e{}, red_e{}, green_e{}, yellow_e{}, red_e{});
    for(int i = 0; i < 5; i++) fsm.proccess();
    ASSERT_TRUE(fsm.is_state(red_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestPriorityToNormalTransition) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.push_events(priority_e{}, green_e{}, yellow_e{});
    for(int i = 0; i < 3; i++) fsm.proccess();
    ASSERT_TRUE(fsm.is_state(yellow_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestMultipleYellowTransitions) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.push_events(yellow_e{}, red_e{}, green_e{}, yellow_e{});
    for(int i = 0; i < 4; i++) fsm.proccess();
    ASSERT_TRUE(fsm.is_state(yellow_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestLongEventSequence) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.push_events(yellow_e{}, red_e{}, green_e{}, yellow_e{}, red_e{}, green_e{});
    for(int i = 0; i < 6; i++) fsm.proccess();
    ASSERT_TRUE(fsm.is_state(green_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestPriorityEventAtEnd) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.push_events(yellow_e{}, red_e{}, green_e{}, priority_e{});
    for(int i = 0; i < 4; i++) fsm.proccess();
    ASSERT_TRUE(fsm.is_state(red_s{}));
}

GTEST_TEST(FiniteStateMachineSuite, TestNoStateChangeWithoutProcess) {
    fsm::FiniteStateMachine<fsm_table> fsm;
    fsm.push_events(yellow_e{}, red_e{}, green_e{});
    ASSERT_TRUE(fsm.is_state(green_s{}));
}
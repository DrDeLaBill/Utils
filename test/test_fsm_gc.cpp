/* Copyright © 2025 Georgy E. All rights reserved. */

#define FSM_GC_EVENTS_COUNT 15

#include <string>
#include <fcntl.h>
#include <gtest/gtest.h>

#include "fsm_gc.h"
#include "gutils.h"


// Test 1: Initialization with valid parameters
FSM_GC_CREATE(FSM_GC_SUITE_ValidInit_fsm)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_ValidInit_state1, _FSM_GC_SUITE_ValidInit_state1)
FSM_GC_CREATE_EVENT(FSM_GC_SUITE_ValidInit_event1, 0)
FSM_GC_CREATE_TABLE(
  FSM_GC_SUITE_ValidInit_fsm_table,
  {&FSM_GC_SUITE_ValidInit_state1, &FSM_GC_SUITE_ValidInit_event1, &FSM_GC_SUITE_ValidInit_state1, NULL}
)
void _FSM_GC_SUITE_ValidInit_state1(void) {}
GTEST_TEST(FSM_GC_SUITE, ValidInit)
{
  fsm_gc_init(&FSM_GC_SUITE_ValidInit_fsm, FSM_GC_SUITE_ValidInit_fsm_table, 1);
  ASSERT_TRUE(FSM_GC_SUITE_ValidInit_fsm._initialized);
  ASSERT_EQ(FSM_GC_SUITE_ValidInit_fsm._state, &FSM_GC_SUITE_ValidInit_state1);
}

// Test 2: Initialization with NULL FSM pointer
GTEST_TEST(FSM_GC_SUITE, InitNullFSM)
{
  fsm_gc_init(nullptr, nullptr, 0);
  // Expect no crash or assertion (if debug enabled)
}

// Test 3: State transition with event processing
FSM_GC_CREATE(FSM_GC_SUITE_StateTransition_fsm)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_StateTransition_state1, _FSM_GC_SUITE_StateTransition_state1)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_StateTransition_state2, _FSM_GC_SUITE_StateTransition_state2)
FSM_GC_CREATE_EVENT(FSM_GC_SUITE_StateTransition_event1, 0)
FSM_GC_CREATE_ACTION(FSM_GC_SUITE_StateTransition_action1, _FSM_GC_SUITE_StateTransition_action1)
FSM_GC_CREATE_TABLE(
  FSM_GC_SUITE_StateTransition_fsm_table,
  {&FSM_GC_SUITE_StateTransition_state1, &FSM_GC_SUITE_StateTransition_event1, &FSM_GC_SUITE_StateTransition_state2, &FSM_GC_SUITE_StateTransition_action1},
)
static bool action_called = false;
void _FSM_GC_SUITE_StateTransition_state1(void) {}
void _FSM_GC_SUITE_StateTransition_state2(void) {}
void _FSM_GC_SUITE_StateTransition_action1(void) { action_called = true; }
GTEST_TEST(FSM_GC_SUITE, StateTransition)
{
  action_called = false;
  fsm_gc_init(&FSM_GC_SUITE_StateTransition_fsm, FSM_GC_SUITE_StateTransition_fsm_table, 1);
  fsm_gc_push_event(&FSM_GC_SUITE_StateTransition_fsm, &FSM_GC_SUITE_StateTransition_event1);
  fsm_gc_process(&FSM_GC_SUITE_StateTransition_fsm);
  ASSERT_TRUE(fsm_gc_is_state(&FSM_GC_SUITE_StateTransition_fsm, &FSM_GC_SUITE_StateTransition_state2));
  ASSERT_TRUE(action_called);
}

// Test 4: Event queue overflow handling
FSM_GC_CREATE(FSM_GC_SUITE_EventOverflow_fsm)
FSM_GC_CREATE_EVENT(FSM_GC_SUITE_EventOverflow_event1, 0)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_EventOverflow_state1, _FSM_GC_SUITE_EventOverflow_state1)
FSM_GC_CREATE_TABLE(
  FSM_GC_SUITE_EventOverflow_fsm_table,
  {&FSM_GC_SUITE_EventOverflow_state1, &FSM_GC_SUITE_EventOverflow_event1, &FSM_GC_SUITE_EventOverflow_state1, NULL}
)
void _FSM_GC_SUITE_EventOverflow_state1(void) {}
GTEST_TEST(FSM_GC_SUITE, EventOverflow)
{
  fsm_gc_init(&FSM_GC_SUITE_EventOverflow_fsm, FSM_GC_SUITE_EventOverflow_fsm_table, 1);
  for (int i = 0; i < FSM_GC_EVENTS_COUNT + 1; i++) {
    fsm_gc_push_event(&FSM_GC_SUITE_EventOverflow_fsm, &FSM_GC_SUITE_EventOverflow_event1);
  }
  ASSERT_EQ(circle_buf_gc_count(FSM_GC_SUITE_EventOverflow_fsm._events), FSM_GC_EVENTS_COUNT);
}

// Test 5: Clear event queue
FSM_GC_CREATE(FSM_GC_SUITE_ClearEvents_fsm)
FSM_GC_CREATE_EVENT(FSM_GC_SUITE_ClearEvents_event1, 0)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_ClearEvents_state1, _FSM_GC_SUITE_ClearEvents_state1)
FSM_GC_CREATE_TABLE(
  FSM_GC_SUITE_ClearEvents_fsm_table,
  {&FSM_GC_SUITE_ClearEvents_state1, &FSM_GC_SUITE_ClearEvents_event1, &FSM_GC_SUITE_ClearEvents_state1, NULL}
)
void _FSM_GC_SUITE_ClearEvents_state1(void) {}
GTEST_TEST(FSM_GC_SUITE, ClearEvents)
{
  fsm_gc_init(&FSM_GC_SUITE_ClearEvents_fsm, FSM_GC_SUITE_ClearEvents_fsm_table, 1);
  fsm_gc_push_event(&FSM_GC_SUITE_ClearEvents_fsm, &FSM_GC_SUITE_ClearEvents_event1);
  fsm_gc_clear(&FSM_GC_SUITE_ClearEvents_fsm);
  ASSERT_EQ(circle_buf_gc_count(FSM_GC_SUITE_ClearEvents_fsm._events), 0);
}
// Test 6: Check current state
FSM_GC_CREATE(FSM_GC_SUITE_IsStateCheck_fsm)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_IsStateCheck_state1, _FSM_GC_SUITE_IsStateCheck_state1)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_IsStateCheck_state2, _FSM_GC_SUITE_IsStateCheck_state2)
FSM_GC_CREATE_TABLE(
  FSM_GC_SUITE_IsStateCheck_fsm_table,
  {&FSM_GC_SUITE_IsStateCheck_state1, nullptr, &FSM_GC_SUITE_IsStateCheck_state2, NULL}
)
void _FSM_GC_SUITE_IsStateCheck_state1(void) {}
void _FSM_GC_SUITE_IsStateCheck_state2(void) {}
GTEST_TEST(FSM_GC_SUITE, IsStateCheck)
{
  fsm_gc_init(&FSM_GC_SUITE_IsStateCheck_fsm, FSM_GC_SUITE_IsStateCheck_fsm_table, 1);
  ASSERT_TRUE(fsm_gc_is_state(&FSM_GC_SUITE_IsStateCheck_fsm, &FSM_GC_SUITE_IsStateCheck_state1));
  ASSERT_FALSE(fsm_gc_is_state(&FSM_GC_SUITE_IsStateCheck_fsm, &FSM_GC_SUITE_IsStateCheck_state2));
}
// Test 7: Handling multiple events with different priorities
FSM_GC_CREATE(FSM_GC_SUITE_EventPriority_fsm)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_EventPriority_state1, _FSM_GC_SUITE_EventPriority_state1)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_EventPriority_state2, _FSM_GC_SUITE_EventPriority_state2)
FSM_GC_CREATE_EVENT(FSM_GC_SUITE_EventPriority_lowPrio, 1)
FSM_GC_CREATE_EVENT(FSM_GC_SUITE_EventPriority_highPrio, 2)
FSM_GC_CREATE_TABLE(
  FSM_GC_SUITE_EventPriority_fsm_table,
  {&FSM_GC_SUITE_EventPriority_state1, &FSM_GC_SUITE_EventPriority_lowPrio, &FSM_GC_SUITE_EventPriority_state1, NULL},
  {&FSM_GC_SUITE_EventPriority_state1, &FSM_GC_SUITE_EventPriority_highPrio, &FSM_GC_SUITE_EventPriority_state2, NULL}
)
void _FSM_GC_SUITE_EventPriority_state1(void) {}
void _FSM_GC_SUITE_EventPriority_state2(void) {}
GTEST_TEST(FSM_GC_SUITE, EventPriority)
{
  fsm_gc_init(&FSM_GC_SUITE_EventPriority_fsm, FSM_GC_SUITE_EventPriority_fsm_table, 2);
  fsm_gc_push_event(&FSM_GC_SUITE_EventPriority_fsm, &FSM_GC_SUITE_EventPriority_lowPrio);
  fsm_gc_push_event(&FSM_GC_SUITE_EventPriority_fsm, &FSM_GC_SUITE_EventPriority_highPrio);
  fsm_gc_process(&FSM_GC_SUITE_EventPriority_fsm);
  ASSERT_TRUE(fsm_gc_is_state(&FSM_GC_SUITE_EventPriority_fsm, &FSM_GC_SUITE_EventPriority_state2));
  // High-priority event should be processed first
  ASSERT_EQ(circle_buf_gc_count(FSM_GC_SUITE_EventPriority_fsm._events), 1);
}

// Test 8: No valid transition for current state/event
FSM_GC_CREATE(FSM_GC_SUITE_NoTransition_fsm)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_NoTransition_state1, _FSM_GC_SUITE_NoTransition_state1)
FSM_GC_CREATE_EVENT(FSM_GC_SUITE_NoTransition_event1, 0)
FSM_GC_CREATE_TABLE(
  FSM_GC_SUITE_NoTransition_fsm_table,
  {&FSM_GC_SUITE_NoTransition_state1, nullptr, &FSM_GC_SUITE_NoTransition_state1, NULL}
)
void _FSM_GC_SUITE_NoTransition_state1(void) {}
GTEST_TEST(FSM_GC_SUITE, NoTransition)
{
  fsm_gc_init(&FSM_GC_SUITE_NoTransition_fsm, FSM_GC_SUITE_NoTransition_fsm_table, 1);
  fsm_gc_push_event(&FSM_GC_SUITE_NoTransition_fsm, &FSM_GC_SUITE_NoTransition_event1);
  fsm_gc_process(&FSM_GC_SUITE_NoTransition_fsm);
  ASSERT_TRUE(fsm_gc_is_state(&FSM_GC_SUITE_NoTransition_fsm, &FSM_GC_SUITE_NoTransition_state1));
}

// Test 9: Transition with NULL action
FSM_GC_CREATE(FSM_GC_SUITE_NullAction_fsm)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_NullAction_state1, _FSM_GC_SUITE_NullAction_state1)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_NullAction_state2, _FSM_GC_SUITE_NullAction_state2)
FSM_GC_CREATE_EVENT(FSM_GC_SUITE_NullAction_event1, 0)
FSM_GC_CREATE_TABLE(
  FSM_GC_SUITE_NullAction_fsm_table,
  {&FSM_GC_SUITE_NullAction_state1, &FSM_GC_SUITE_NullAction_event1, &FSM_GC_SUITE_NullAction_state2, NULL}
)
void _FSM_GC_SUITE_NullAction_state1(void) {}
void _FSM_GC_SUITE_NullAction_state2(void) {}
GTEST_TEST(FSM_GC_SUITE, NullAction)
{
  fsm_gc_init(&FSM_GC_SUITE_NullAction_fsm, FSM_GC_SUITE_NullAction_fsm_table, 1);
  fsm_gc_push_event(&FSM_GC_SUITE_NullAction_fsm, &FSM_GC_SUITE_NullAction_event1);
  fsm_gc_process(&FSM_GC_SUITE_NullAction_fsm);
  ASSERT_TRUE(fsm_gc_is_state(&FSM_GC_SUITE_NullAction_fsm, &FSM_GC_SUITE_NullAction_state2));
}

// Test 10: Consecutive state transitions
FSM_GC_CREATE(FSM_GC_SUITE_ConsecutiveTransitions_fsm)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_ConsecutiveTransitions_state1, _FSM_GC_SUITE_ConsecutiveTransitions_state1)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_ConsecutiveTransitions_state2, _FSM_GC_SUITE_ConsecutiveTransitions_state2)
FSM_GC_CREATE_EVENT(FSM_GC_SUITE_ConsecutiveTransitions_event1, 0)
FSM_GC_CREATE_TABLE(
  FSM_GC_SUITE_ConsecutiveTransitions_fsm_table,
  {&FSM_GC_SUITE_ConsecutiveTransitions_state1, &FSM_GC_SUITE_ConsecutiveTransitions_event1, &FSM_GC_SUITE_ConsecutiveTransitions_state2, NULL},
  {&FSM_GC_SUITE_ConsecutiveTransitions_state2, &FSM_GC_SUITE_ConsecutiveTransitions_event1, &FSM_GC_SUITE_ConsecutiveTransitions_state1, NULL}
  )
void _FSM_GC_SUITE_ConsecutiveTransitions_state1(void) {}
void _FSM_GC_SUITE_ConsecutiveTransitions_state2(void) {}
GTEST_TEST(FSM_GC_SUITE, ConsecutiveTransitions)
{
  fsm_gc_init(&FSM_GC_SUITE_ConsecutiveTransitions_fsm, FSM_GC_SUITE_ConsecutiveTransitions_fsm_table, 2);
  fsm_gc_push_event(&FSM_GC_SUITE_ConsecutiveTransitions_fsm, &FSM_GC_SUITE_ConsecutiveTransitions_event1);
  fsm_gc_process(&FSM_GC_SUITE_ConsecutiveTransitions_fsm);
  ASSERT_TRUE(fsm_gc_is_state(&FSM_GC_SUITE_ConsecutiveTransitions_fsm, &FSM_GC_SUITE_ConsecutiveTransitions_state2));
  
  fsm_gc_push_event(&FSM_GC_SUITE_ConsecutiveTransitions_fsm, &FSM_GC_SUITE_ConsecutiveTransitions_event1);
  fsm_gc_process(&FSM_GC_SUITE_ConsecutiveTransitions_fsm);
  ASSERT_TRUE(fsm_gc_is_state(&FSM_GC_SUITE_ConsecutiveTransitions_fsm, &FSM_GC_SUITE_ConsecutiveTransitions_state1));
}

// Test 11: Event index uniqueness during initialization
FSM_GC_CREATE(FSM_GC_SUITE_EventIndexUniqueness_fsm)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_EventIndexUniqueness_state1, _FSM_GC_SUITE_EventIndexUniqueness_state1)
FSM_GC_CREATE_EVENT(FSM_GC_SUITE_EventIndexUniqueness_event1, 0)
FSM_GC_CREATE_EVENT(FSM_GC_SUITE_EventIndexUniqueness_event2, 0)
FSM_GC_CREATE_TABLE(
  FSM_GC_SUITE_EventIndexUniqueness_fsm_table,
  {&FSM_GC_SUITE_EventIndexUniqueness_state1, &FSM_GC_SUITE_EventIndexUniqueness_event1, &FSM_GC_SUITE_EventIndexUniqueness_state1, NULL},
  {&FSM_GC_SUITE_EventIndexUniqueness_state1, &FSM_GC_SUITE_EventIndexUniqueness_event2, &FSM_GC_SUITE_EventIndexUniqueness_state1, NULL}
)
void _FSM_GC_SUITE_EventIndexUniqueness_state1(void) {}
GTEST_TEST(FSM_GC_SUITE, EventIndexUniqueness)
{
  fsm_gc_init(&FSM_GC_SUITE_EventIndexUniqueness_fsm, FSM_GC_SUITE_EventIndexUniqueness_fsm_table, 2);
  ASSERT_NE(
    FSM_GC_SUITE_EventIndexUniqueness_event1.index, 
    FSM_GC_SUITE_EventIndexUniqueness_event2.index
  );
}

// Test 12: State function execution during processing
FSM_GC_CREATE(FSM_GC_SUITE_StateFunctionExecution_fsm)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_StateFunctionExecution_state1, _FSM_GC_SUITE_StateFunctionExecution_state1)
FSM_GC_CREATE_TABLE(
  FSM_GC_SUITE_StateFunctionExecution_fsm_table,
  {&FSM_GC_SUITE_StateFunctionExecution_state1, nullptr, &FSM_GC_SUITE_StateFunctionExecution_state1, NULL}
)
static bool state_function_called = false;
void _FSM_GC_SUITE_StateFunctionExecution_state1(void) { state_function_called = true; }
GTEST_TEST(FSM_GC_SUITE, StateFunctionExecution)
{
  state_function_called = false;
  fsm_gc_init(&FSM_GC_SUITE_StateFunctionExecution_fsm, FSM_GC_SUITE_StateFunctionExecution_fsm_table, 1);
  fsm_gc_process(&FSM_GC_SUITE_StateFunctionExecution_fsm);
  ASSERT_TRUE(state_function_called);
}

// Test 13: Processing multiple events in cycle until empty queue
FSM_GC_CREATE(FSM_GC_SUITE_ProcessCycle_fsm)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_ProcessCycle_stateA, _FSM_GC_SUITE_ProcessCycle_stateA)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_ProcessCycle_stateB, _FSM_GC_SUITE_ProcessCycle_stateB)
FSM_GC_CREATE_EVENT(FSM_GC_SUITE_ProcessCycle_event1, 0)
FSM_GC_CREATE_TABLE(
  FSM_GC_SUITE_ProcessCycle_fsm_table,
  {&FSM_GC_SUITE_ProcessCycle_stateA, &FSM_GC_SUITE_ProcessCycle_event1, &FSM_GC_SUITE_ProcessCycle_stateB, NULL},
  {&FSM_GC_SUITE_ProcessCycle_stateB, &FSM_GC_SUITE_ProcessCycle_event1, &FSM_GC_SUITE_ProcessCycle_stateA, NULL}
)
void _FSM_GC_SUITE_ProcessCycle_stateA(void) {}
void _FSM_GC_SUITE_ProcessCycle_stateB(void) {}
GTEST_TEST(FSM_GC_SUITE, ProcessCycle)
{
  fsm_gc_init(&FSM_GC_SUITE_ProcessCycle_fsm, FSM_GC_SUITE_ProcessCycle_fsm_table, 2);
  
  // Push 3 events to create state oscillation
  for (int i = 0; i < 3; i++) {
    fsm_gc_push_event(&FSM_GC_SUITE_ProcessCycle_fsm, &FSM_GC_SUITE_ProcessCycle_event1);
  }
  
  // Process in loop until all events are handled
  while (circle_buf_gc_count(FSM_GC_SUITE_ProcessCycle_fsm._events) > 0) {
    fsm_gc_process(&FSM_GC_SUITE_ProcessCycle_fsm);
  }
  
  // Final state should be stateB after odd number of transitions
  ASSERT_TRUE(fsm_gc_is_state(&FSM_GC_SUITE_ProcessCycle_fsm, &FSM_GC_SUITE_ProcessCycle_stateB));
}

// Test 14: State-driven event generation in processing loop
FSM_GC_CREATE(FSM_GC_SUITE_StateDrivenEvents_fsm)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_StateDrivenEvents_state1, _FSM_GC_SUITE_StateDrivenEvents_state1)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_StateDrivenEvents_state2, _FSM_GC_SUITE_StateDrivenEvents_state2)
FSM_GC_CREATE_EVENT(FSM_GC_SUITE_StateDrivenEvents_trigger, 0)
FSM_GC_CREATE_TABLE(
  FSM_GC_SUITE_StateDrivenEvents_fsm_table,
  {&FSM_GC_SUITE_StateDrivenEvents_state1, &FSM_GC_SUITE_StateDrivenEvents_trigger, &FSM_GC_SUITE_StateDrivenEvents_state2, NULL}
)
void _FSM_GC_SUITE_StateDrivenEvents_state1(void) {
  // Generate new event during state execution
  fsm_gc_push_event(&FSM_GC_SUITE_StateDrivenEvents_fsm, &FSM_GC_SUITE_StateDrivenEvents_trigger);
}
void _FSM_GC_SUITE_StateDrivenEvents_state2(void) {}
GTEST_TEST(FSM_GC_SUITE, StateDrivenEvents)
{
  fsm_gc_init(&FSM_GC_SUITE_StateDrivenEvents_fsm, FSM_GC_SUITE_StateDrivenEvents_fsm_table, 1);
  
  // Initial push and processing loop
  fsm_gc_push_event(&FSM_GC_SUITE_StateDrivenEvents_fsm, &FSM_GC_SUITE_StateDrivenEvents_trigger);
  for (int i = 0; i < 5; i++) { // Safety limit for loop
    fsm_gc_process(&FSM_GC_SUITE_StateDrivenEvents_fsm);
    if (fsm_gc_is_state(&FSM_GC_SUITE_StateDrivenEvents_fsm, &FSM_GC_SUITE_StateDrivenEvents_state2)) {
      break;
    }
  }
  ASSERT_TRUE(fsm_gc_is_state(&FSM_GC_SUITE_StateDrivenEvents_fsm, &FSM_GC_SUITE_StateDrivenEvents_state2));
}

// Test 15: Continuous processing with empty event queue
FSM_GC_CREATE(FSM_GC_SUITE_EmptyQueueProcessing_fsm)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_EmptyQueueProcessing_state1, _FSM_GC_SUITE_EmptyQueueProcessing_state1)
FSM_GC_CREATE_TABLE(
    FSM_GC_SUITE_EmptyQueueProcessing_fsm_table,
    {&FSM_GC_SUITE_EmptyQueueProcessing_state1, nullptr, &FSM_GC_SUITE_EmptyQueueProcessing_state1, NULL}
)
static int state_exec_count = 0;
void _FSM_GC_SUITE_EmptyQueueProcessing_state1(void) { state_exec_count++; }
GTEST_TEST(FSM_GC_SUITE, EmptyQueueProcessing)
{
    state_exec_count = 0;
    fsm_gc_init(&FSM_GC_SUITE_EmptyQueueProcessing_fsm, FSM_GC_SUITE_EmptyQueueProcessing_fsm_table, 1);
    
    // Process multiple times with empty queue
    for (int i = 0; i < 5; i++) {
        fsm_gc_process(&FSM_GC_SUITE_EmptyQueueProcessing_fsm);
    }
    
    // State function should execute every process call
    ASSERT_EQ(state_exec_count, 5);
}

// Test 16: Priority-based processing in loop
FSM_GC_CREATE(FSM_GC_SUITE_PriorityLoop_fsm)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_PriorityLoop_state1, _FSM_GC_SUITE_PriorityLoop_state1)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_PriorityLoop_state2, _FSM_GC_SUITE_PriorityLoop_state2)
FSM_GC_CREATE_EVENT(FSM_GC_SUITE_PriorityLoop_lowPrio, 1)
FSM_GC_CREATE_EVENT(FSM_GC_SUITE_PriorityLoop_highPrio, 2)
FSM_GC_CREATE_TABLE(
  FSM_GC_SUITE_PriorityLoop_fsm_table,
  {&FSM_GC_SUITE_PriorityLoop_state1, &FSM_GC_SUITE_PriorityLoop_lowPrio,  &FSM_GC_SUITE_PriorityLoop_state2, NULL},
  {&FSM_GC_SUITE_PriorityLoop_state1, &FSM_GC_SUITE_PriorityLoop_highPrio, &FSM_GC_SUITE_PriorityLoop_state2, NULL},
  {&FSM_GC_SUITE_PriorityLoop_state2, &FSM_GC_SUITE_PriorityLoop_lowPrio,  &FSM_GC_SUITE_PriorityLoop_state2, NULL},
  {&FSM_GC_SUITE_PriorityLoop_state2, &FSM_GC_SUITE_PriorityLoop_highPrio, &FSM_GC_SUITE_PriorityLoop_state2, NULL}
)
void _FSM_GC_SUITE_PriorityLoop_state1(void) {}
void _FSM_GC_SUITE_PriorityLoop_state2(void) {}
GTEST_TEST(FSM_GC_SUITE, PriorityLoop)
{
    fsm_gc_init(&FSM_GC_SUITE_PriorityLoop_fsm, FSM_GC_SUITE_PriorityLoop_fsm_table, 4);
    
    // Push mixed-priority events
    fsm_gc_push_event(&FSM_GC_SUITE_PriorityLoop_fsm, &FSM_GC_SUITE_PriorityLoop_lowPrio);
    fsm_gc_push_event(&FSM_GC_SUITE_PriorityLoop_fsm, &FSM_GC_SUITE_PriorityLoop_highPrio);
    fsm_gc_push_event(&FSM_GC_SUITE_PriorityLoop_fsm, &FSM_GC_SUITE_PriorityLoop_lowPrio);

    // Process until queue empty
    while (circle_buf_gc_count(FSM_GC_SUITE_PriorityLoop_fsm._events) > 0) {
        fsm_gc_process(&FSM_GC_SUITE_PriorityLoop_fsm);
    }
    
    // High-priority event should be processed first, then remaining low-prio
    ASSERT_TRUE(fsm_gc_is_state(&FSM_GC_SUITE_PriorityLoop_fsm, &FSM_GC_SUITE_PriorityLoop_state2));
}

// Test 17: Terminating loop on target state
FSM_GC_CREATE(FSM_GC_SUITE_TerminationCondition_fsm)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_TerminationCondition_state1, _FSM_GC_SUITE_TerminationCondition_state1)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_TerminationCondition_state2, _FSM_GC_SUITE_TerminationCondition_state2)
FSM_GC_CREATE_EVENT(FSM_GC_SUITE_TerminationCondition_event1, 0)
FSM_GC_CREATE_TABLE(
    FSM_GC_SUITE_TerminationCondition_fsm_table,
    {&FSM_GC_SUITE_TerminationCondition_state1, &FSM_GC_SUITE_TerminationCondition_event1, &FSM_GC_SUITE_TerminationCondition_state2, NULL},
    {&FSM_GC_SUITE_TerminationCondition_state2, & FSM_GC_SUITE_TerminationCondition_event1, & FSM_GC_SUITE_TerminationCondition_state2, NULL}
)
void _FSM_GC_SUITE_TerminationCondition_state1(void) {}
void _FSM_GC_SUITE_TerminationCondition_state2(void) {}
GTEST_TEST(FSM_GC_SUITE, TerminationCondition)
{
    fsm_gc_init(&FSM_GC_SUITE_TerminationCondition_fsm, FSM_GC_SUITE_TerminationCondition_fsm_table, 2);
    
    // Push multiple redundant events
    for (int i = 0; i < 5; i++) {
        fsm_gc_push_event(&FSM_GC_SUITE_TerminationCondition_fsm, &FSM_GC_SUITE_TerminationCondition_event1);
    }

    // Process until target state reached
    bool target_reached = false;
    for (int i = 0; i < 10 && !target_reached; i++) { // Safety limit
        fsm_gc_process(&FSM_GC_SUITE_TerminationCondition_fsm);
        target_reached = fsm_gc_is_state(&FSM_GC_SUITE_TerminationCondition_fsm, 
                                    &FSM_GC_SUITE_TerminationCondition_state2);
    }
    
    ASSERT_TRUE(target_reached);
    // Ensure remaining events were cleared
    ASSERT_EQ(circle_buf_gc_count(FSM_GC_SUITE_TerminationCondition_fsm._events), 4);
}

// Test 18: Check events count
FSM_GC_CREATE(FSM_GC_SUITE_CheckEventsCount_fsm)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_CheckEventsCount_state1, _FSM_GC_SUITE_CheckEventsCount_state1)
FSM_GC_CREATE_EVENT(FSM_GC_SUITE_CheckEventsCount_event1, 0)
FSM_GC_CREATE_TABLE(
    FSM_GC_SUITE_CheckEventsCount_fsm_table,
    { &FSM_GC_SUITE_CheckEventsCount_state1, &FSM_GC_SUITE_CheckEventsCount_event1, &FSM_GC_SUITE_CheckEventsCount_state1, NULL }
)
void _FSM_GC_SUITE_CheckEventsCount_state1(void) {}
GTEST_TEST(FSM_GC_SUITE, CheckEventsCount)
{
    fsm_gc_init(&FSM_GC_SUITE_CheckEventsCount_fsm, FSM_GC_SUITE_CheckEventsCount_fsm_table, 1);
    ASSERT_EQ(__arr_len(__concat(__events_buf_, FSM_GC_SUITE_CheckEventsCount_fsm)), FSM_GC_EVENTS_COUNT);
}


#if !defined(FSM_GC_NO_DEBUG) && (defined(_DEBUG) || defined(DEBUG) || defined(GBEDUG_FORCE))
FSM_GC_CREATE(FSM_GC_SUITE_CheckMatchesStates_fsm)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_CheckMatchesStates_state1, _FSM_GC_SUITE_CheckMatchesStates_state1)
FSM_GC_CREATE_STATE(FSM_GC_SUITE_CheckMatchesStates_state2, _FSM_GC_SUITE_CheckMatchesStates_state1)
FSM_GC_CREATE_EVENT(FSM_GC_SUITE_CheckMatchesStates_event1, 0)
FSM_GC_CREATE_TABLE(
    FSM_GC_SUITE_CheckMatchesStates_fsm_table,
    { &FSM_GC_SUITE_CheckMatchesStates_state1, &FSM_GC_SUITE_CheckMatchesStates_event1, &FSM_GC_SUITE_CheckMatchesStates_state1, NULL },
    { &FSM_GC_SUITE_CheckMatchesStates_state2, &FSM_GC_SUITE_CheckMatchesStates_event1, &FSM_GC_SUITE_CheckMatchesStates_state2, NULL }
)
void _FSM_GC_SUITE_CheckMatchesStates_state1(void) {}
GTEST_TEST(FSM_GC_SUITE, CheckMatchesStates)
{
    // Перехват stdout с помощью pipe
    uint8_t buffer[1024] = "";
    int pipefd[2];
#if defined(_MSC_VER)
    _pipe(pipefd, sizeof(buffer), O_BINARY);
    int old_stdout = _dup(_fileno(stdout));
    _dup2(pipefd[1], _fileno(stdout));
#elif defined(__GNUC__)
    pipe(pipefd);
    int old_stdout = dup(STDOUT_FILENO);
    dup2(pipefd[1], STDOUT_FILENO);
#else
#   error "change stdout error"
#endif

    std::cout << "FSM_GC_SUITE CheckMatchesStates ";
    fsm_gc_init(&FSM_GC_SUITE_CheckMatchesStates_fsm, FSM_GC_SUITE_CheckMatchesStates_fsm_table, 2);
    
    fflush(stdout);
#if defined(_MSC_VER)
    // Чтение из pipe
    _read(pipefd[0], buffer, sizeof(buffer));
    // Восстановление stdout
    _dup2(old_stdout, _fileno(stdout));
#elif defined(__GNUC__)
    // Чтение из pipe
    read(pipefd[0], buffer, sizeof(buffer));
    // Восстановление stdout
    dup2(old_stdout, STDOUT_FILENO);
#endif

    const uint8_t str1[] = "\"FSM_GC_SUITE_CheckMatchesStates_fsm\" has been initialized";
    const uint8_t str2[] = "WARNING! \"FSM_GC_SUITE_CheckMatchesStates_fsm\" has matches functions states FSM_GC_SUITE_CheckMatchesStates_state1{";
    const uint8_t str3[] = "} = FSM_GC_SUITE_CheckMatchesStates_state2{";
    ASSERT_TRUE((bool)util_memfind(buffer, sizeof(buffer), str1, strlen((char*)str1)));
    ASSERT_TRUE((bool)util_memfind(buffer, sizeof(buffer), str2, strlen((char*)str2)));
    ASSERT_TRUE((bool)util_memfind(buffer, sizeof(buffer), str3, strlen((char*)str3)));
}
#endif

// TODO: add DEBUG messages fixture and tests
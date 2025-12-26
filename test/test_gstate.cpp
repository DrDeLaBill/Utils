/* Copyright © 2025 Georgy E. All rights reserved. */

#include <gtest/gtest.h>

#include "gstate.h"
#include "gutils.h"
#include "circle_buf_gc.h"


class GSTATE_Fixture: public testing::Test
{
public:
    void SetUp() override {}
    void TearDown() override 
    {
        gstate_enable_all_messages();
    }
};


// Test 1: Initialization with valid parameters
GSTATE_CREATE_STATE(GSV_ValidInit_state1_s, _GSV_ValidInit_state1_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_ValidInit_states, GSV_ValidInit_state1_s);
GSTATE_CREATE(GSTATE_SUITE_ValidInit_gstate)
TEST(GSTATE_Fixture, ValidInit)
{
    gstate_init(&GSTATE_SUITE_ValidInit_gstate, GSTATE_SUITE_ValidInit_states, __arr_len(GSTATE_SUITE_ValidInit_states));
    ASSERT_TRUE(GSTATE_SUITE_ValidInit_gstate._initialized);
    ASSERT_EQ(GSTATE_SUITE_ValidInit_gstate._curr_state->state_f, GSV_ValidInit_state1_s.state_f);
}

// Test 2: Initialization with NULL GSTATE pointer
TEST(GSTATE_Fixture, InitNullGState)
{
    gstate_init(nullptr, nullptr, 0);
    // Expect no crash or assertion (if debug enabled)
}

// Test 3: State transition with queued request
GSTATE_CREATE_STATE(GSV_StateTransition_state1_s, _GSV_StateTransition_state1_s, 0);
GSTATE_CREATE_STATE(GSV_StateTransition_state2_s, _GSV_StateTransition_state2_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_StateTransition_states, GSV_StateTransition_state1_s, GSV_StateTransition_state2_s);
GSTATE_CREATE(GSTATE_SUITE_StateTransition_gstate)
TEST(GSTATE_Fixture, StateTransition)
{
    gstate_init(&GSTATE_SUITE_StateTransition_gstate, GSTATE_SUITE_StateTransition_states, __arr_len(GSTATE_SUITE_StateTransition_states));
    gstate_request_state(&GSTATE_SUITE_StateTransition_gstate, &GSV_StateTransition_state2_s);
    gstate_process(&GSTATE_SUITE_StateTransition_gstate);
    ASSERT_TRUE(gstate_is_state(&GSTATE_SUITE_StateTransition_gstate, &GSV_StateTransition_state2_s));
}

// Test 4: Clear queue
GSTATE_CREATE_STATE(GSV_ClearQueue_state1_s, _GSV_ClearQueue_state1_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_ClearQueue_states, GSV_ClearQueue_state1_s);
GSTATE_CREATE(GSTATE_SUITE_ClearQueue_gstate)
TEST(GSTATE_Fixture, ClearQueue)
{
    gstate_init(&GSTATE_SUITE_ClearQueue_gstate, GSTATE_SUITE_ClearQueue_states, __arr_len(GSTATE_SUITE_ClearQueue_states));
    gstate_request_state(&GSTATE_SUITE_ClearQueue_gstate, &GSV_ClearQueue_state1_s);
    gstate_clear(&GSTATE_SUITE_ClearQueue_gstate);
    ASSERT_EQ(circle_buf_gc_count(GSTATE_SUITE_ClearQueue_gstate._states_queue), 0);
}

// Test 5: Check current state
GSTATE_CREATE_STATE(GSV_IsStateCheck_state1_s, _GSV_IsStateCheck_state1_s, 0);
GSTATE_CREATE_STATE(GSV_IsStateCheck_state2_s, _GSV_IsStateCheck_state2_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_IsStateCheck_states, GSV_IsStateCheck_state1_s, GSV_IsStateCheck_state2_s);
GSTATE_CREATE(GSTATE_SUITE_IsStateCheck_gstate)
TEST(GSTATE_Fixture, IsStateCheck)
{
    gstate_init(&GSTATE_SUITE_IsStateCheck_gstate, GSTATE_SUITE_IsStateCheck_states, __arr_len(GSTATE_SUITE_IsStateCheck_states));
    ASSERT_TRUE(gstate_is_state(&GSTATE_SUITE_IsStateCheck_gstate, &GSV_IsStateCheck_state1_s));
    ASSERT_FALSE(gstate_is_state(&GSTATE_SUITE_IsStateCheck_gstate, &GSV_IsStateCheck_state2_s));
}

// Test 6: PrioritySelection
GSTATE_CREATE_STATE(GSV_Priority_state1_s, _GSV_Priority_state1_s, 0);
GSTATE_CREATE_STATE(GSV_Priority_state2_s, _GSV_Priority_state2_s, 2);
GSTATE_CREATE_STATE(GSV_Priority_state3_s, _GSV_Priority_state3_s, 1);
GSTATE_CREATE_LIST(GSTATE_SUITE_Priority_states, GSV_Priority_state1_s, GSV_Priority_state2_s, GSV_Priority_state3_s);
GSTATE_CREATE(GSTATE_SUITE_Priority_gstate)
TEST(GSTATE_Fixture, PrioritySelection)
{
    gstate_init(&GSTATE_SUITE_Priority_gstate, GSTATE_SUITE_Priority_states, __arr_len(GSTATE_SUITE_Priority_states));
    gstate_request_state(&GSTATE_SUITE_Priority_gstate, &GSV_Priority_state2_s);
    gstate_request_state(&GSTATE_SUITE_Priority_gstate, &GSV_Priority_state3_s);
    gstate_process(&GSTATE_SUITE_Priority_gstate);
    ASSERT_TRUE(gstate_is_state(&GSTATE_SUITE_Priority_gstate, &GSV_Priority_state3_s));
    ASSERT_EQ(circle_buf_gc_count(GSTATE_SUITE_Priority_gstate._states_queue), 0);
}

// Test 7: NoTransition
GSTATE_CREATE_STATE(GSV_NoTransition_state1_s, _GSV_NoTransition_state1_s, 0);
GSTATE_CREATE_STATE(GSV_NoTransition_fake_s, _GSV_NoTransition_fake_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_NoTransition_states, GSV_NoTransition_state1_s);
GSTATE_CREATE(GSTATE_SUITE_NoTransition_gstate)
TEST(GSTATE_Fixture, NoTransition)
{
    gstate_init(&GSTATE_SUITE_NoTransition_gstate, GSTATE_SUITE_NoTransition_states, __arr_len(GSTATE_SUITE_NoTransition_states));
    gstate_request_state(&GSTATE_SUITE_NoTransition_gstate, &GSV_NoTransition_fake_s);
    gstate_process(&GSTATE_SUITE_NoTransition_gstate);
    ASSERT_TRUE(gstate_is_state(&GSTATE_SUITE_NoTransition_gstate, &GSV_NoTransition_state1_s));
}

// Test 8: StateFunctionExecution
static bool GSV_StateFunctionExecution_called = false;
GSTATE_CREATE_STATE(GSV_StateFunctionExecution_state1_s, _GSV_StateFunctionExecution_state1_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_StateFunctionExecution_states, GSV_StateFunctionExecution_state1_s);
GSTATE_CREATE(GSTATE_SUITE_StateFunctionExecution_gstate)
TEST(GSTATE_Fixture, StateFunctionExecution)
{
    GSV_StateFunctionExecution_called = false;
    gstate_init(&GSTATE_SUITE_StateFunctionExecution_gstate, GSTATE_SUITE_StateFunctionExecution_states, __arr_len(GSTATE_SUITE_StateFunctionExecution_states));
    gstate_process(&GSTATE_SUITE_StateFunctionExecution_gstate);
    ASSERT_TRUE(GSV_StateFunctionExecution_called);
}

// Test 9: ProcessCycle
GSTATE_CREATE_STATE(GSV_ProcessCycle_stateA_s, _GSV_ProcessCycle_stateA_s, 0);
GSTATE_CREATE_STATE(GSV_ProcessCycle_stateB_s, _GSV_ProcessCycle_stateB_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_ProcessCycle_states, GSV_ProcessCycle_stateA_s, GSV_ProcessCycle_stateB_s);
GSTATE_CREATE(GSTATE_SUITE_ProcessCycle_gstate)
TEST(GSTATE_Fixture, ProcessCycle)
{
    gstate_init(&GSTATE_SUITE_ProcessCycle_gstate, GSTATE_SUITE_ProcessCycle_states, __arr_len(GSTATE_SUITE_ProcessCycle_states));
    for (int i = 0; i < 3; ++i) {
        gstate_request_state(&GSTATE_SUITE_ProcessCycle_gstate, &GSV_ProcessCycle_stateB_s);
        gstate_process(&GSTATE_SUITE_ProcessCycle_gstate);
    }
    ASSERT_TRUE(gstate_is_state(&GSTATE_SUITE_ProcessCycle_gstate, &GSV_ProcessCycle_stateB_s));
}

// Test 10: StateDrivenRequests
GSTATE_CREATE_STATE(GSV_StateDriven_state1_s, _GSV_StateDriven_state1_s, 0);
GSTATE_CREATE_STATE(GSV_StateDriven_state2_s, _GSV_StateDriven_state2_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_StateDriven_states, GSV_StateDriven_state1_s, GSV_StateDriven_state2_s);
GSTATE_CREATE(GSTATE_SUITE_StateDriven_gstate)
TEST(GSTATE_Fixture, StateDrivenRequests)
{
    gstate_init(&GSTATE_SUITE_StateDriven_gstate, GSTATE_SUITE_StateDriven_states, __arr_len(GSTATE_SUITE_StateDriven_states));
    gstate_process(&GSTATE_SUITE_StateDriven_gstate);
    ASSERT_TRUE(gstate_is_state(&GSTATE_SUITE_StateDriven_gstate, &GSV_StateDriven_state2_s));
}

// Test 11: EmptyQueueProcessing
static int GSV_EmptyQueue_exec_count = 0;
GSTATE_CREATE_STATE(GSV_EmptyQueue_state1_s, _GSV_EmptyQueue_state1_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_EmptyQueue_states, GSV_EmptyQueue_state1_s);
GSTATE_CREATE(GSTATE_SUITE_EmptyQueue_gstate)
TEST(GSTATE_Fixture, EmptyQueueProcessing)
{
    GSV_EmptyQueue_exec_count = 0;
    gstate_init(&GSTATE_SUITE_EmptyQueue_gstate, GSTATE_SUITE_EmptyQueue_states, __arr_len(GSTATE_SUITE_EmptyQueue_states));
    for (int i = 0; i < 5; ++i) {
        gstate_process(&GSTATE_SUITE_EmptyQueue_gstate);
    }
    ASSERT_EQ(GSV_EmptyQueue_exec_count, 5);
}

// Test 12: TerminationCondition
GSTATE_CREATE_STATE(GSV_Termination_state1_s, _GSV_Termination_state1_s, 0);
GSTATE_CREATE_STATE(GSV_Termination_state2_s, _GSV_Termination_state2_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_Termination_states, GSV_Termination_state1_s, GSV_Termination_state2_s);
GSTATE_CREATE(GSTATE_SUITE_Termination_gstate)
TEST(GSTATE_Fixture, TerminationCondition)
{
    gstate_init(&GSTATE_SUITE_Termination_gstate, GSTATE_SUITE_Termination_states, __arr_len(GSTATE_SUITE_Termination_states));
    for (int i = 0; i < 5; ++i) {
        gstate_request_state(&GSTATE_SUITE_Termination_gstate, &GSV_Termination_state2_s);
    }
    bool target_reached = false;
    for (int i = 0; i < 10 && !target_reached; ++i) {
        gstate_process(&GSTATE_SUITE_Termination_gstate);
        target_reached = gstate_is_state(&GSTATE_SUITE_Termination_gstate, &GSV_Termination_state2_s);
    }
    ASSERT_TRUE(target_reached);
    ASSERT_EQ(circle_buf_gc_count(GSTATE_SUITE_Termination_gstate._states_queue), 0);
}

// Test 13: CheckStatesCount
GSTATE_CREATE_STATE(GSV_CheckStatesCount_state1_s, _GSV_CheckStatesCount_state1_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_CheckStatesCount_states, GSV_CheckStatesCount_state1_s);
GSTATE_CREATE(GSTATE_SUITE_CheckStatesCount_gstate)
TEST(GSTATE_Fixture, CheckStatesCount)
{
    gstate_init(&GSTATE_SUITE_CheckStatesCount_gstate, GSTATE_SUITE_CheckStatesCount_states, __arr_len(GSTATE_SUITE_CheckStatesCount_states));
    ASSERT_EQ(GSTATE_SUITE_CheckStatesCount_gstate._states_count, 1u);
}

// Unique Test A: Deduplication
GSTATE_CREATE_STATE(GSV_Dedup_stateA_s, _GSV_Dedup_stateA_s, 0);
GSTATE_CREATE_STATE(GSV_Dedup_stateB_s, _GSV_Dedup_stateB_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_Dedup_states, GSV_Dedup_stateA_s, GSV_Dedup_stateB_s, GSV_Dedup_stateA_s /* duplicate */);
GSTATE_CREATE(GSTATE_SUITE_Dedup_gstate)
TEST(GSTATE_Fixture, DeduplicationRemovesDuplicatesAndEmpty)
{
    gstate_init(&GSTATE_SUITE_Dedup_gstate, GSTATE_SUITE_Dedup_states, __arr_len(GSTATE_SUITE_Dedup_states));
    ASSERT_EQ(GSTATE_SUITE_Dedup_gstate._states_count, 2u);
    ASSERT_TRUE(gstate_is_state(&GSTATE_SUITE_Dedup_gstate, &GSV_Dedup_stateA_s));
}

// Unique Test B: RequestNullTargetIsIgnored
GSTATE_CREATE_STATE(GSV_RequestNull_state1_s, _GSV_RequestNull_state1_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_RequestNull_states, GSV_RequestNull_state1_s);
GSTATE_CREATE(GSTATE_SUITE_RequestNull_gstate)
TEST(GSTATE_Fixture, RequestNullTargetIsIgnored)
{
    gstate_init(&GSTATE_SUITE_RequestNull_gstate, GSTATE_SUITE_RequestNull_states, __arr_len(GSTATE_SUITE_RequestNull_states));
    ASSERT_EQ(circle_buf_gc_count(GSTATE_SUITE_RequestNull_gstate._states_queue), 0);
    gstate_request_state(&GSTATE_SUITE_RequestNull_gstate, NULL);
    ASSERT_EQ(circle_buf_gc_count(GSTATE_SUITE_RequestNull_gstate._states_queue), 0);
}

// Unique Test C: RequestingCurrentStateIsIgnored
GSTATE_CREATE_STATE(GSV_RequestSame_state1_s, _GSV_RequestSame_state1_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_RequestSame_states, GSV_RequestSame_state1_s);
GSTATE_CREATE(GSTATE_SUITE_RequestSame_gstate)
TEST(GSTATE_Fixture, RequestingCurrentStateIsIgnored)
{
    gstate_init(&GSTATE_SUITE_RequestSame_gstate, GSTATE_SUITE_RequestSame_states, __arr_len(GSTATE_SUITE_RequestSame_states));
    gstate_request_state(&GSTATE_SUITE_RequestSame_gstate, &GSV_RequestSame_state1_s);
    gstate_process(&GSTATE_SUITE_RequestSame_gstate);
    ASSERT_TRUE(gstate_is_state(&GSTATE_SUITE_RequestSame_gstate, &GSV_RequestSame_state1_s));
    ASSERT_EQ(circle_buf_gc_count(GSTATE_SUITE_RequestSame_gstate._states_queue), 0);
}

// Unique Test D: Reset behavior
GSTATE_CREATE_STATE(GSV_Reset_state1_s, _GSV_Reset_state1_s, 0);
GSTATE_CREATE_STATE(GSV_Reset_state2_s, _GSV_Reset_state2_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_ResetBehavior_states, GSV_Reset_state1_s, GSV_Reset_state2_s);
GSTATE_CREATE(GSTATE_SUITE_ResetBehavior_gstate)
TEST(GSTATE_Fixture, ResetRestoresFirstStateAndClearsQueue)
{
    gstate_init(&GSTATE_SUITE_ResetBehavior_gstate, GSTATE_SUITE_ResetBehavior_states, __arr_len(GSTATE_SUITE_ResetBehavior_states));
    gstate_request_state(&GSTATE_SUITE_ResetBehavior_gstate, &GSV_Reset_state2_s);
    gstate_process(&GSTATE_SUITE_ResetBehavior_gstate);
    ASSERT_TRUE(gstate_is_state(&GSTATE_SUITE_ResetBehavior_gstate, &GSV_Reset_state2_s));
    gstate_reset(&GSTATE_SUITE_ResetBehavior_gstate);
    ASSERT_TRUE(gstate_is_state(&GSTATE_SUITE_ResetBehavior_gstate, &GSV_Reset_state1_s));
    ASSERT_EQ(circle_buf_gc_count(GSTATE_SUITE_ResetBehavior_gstate._states_queue), 0);
}

// Unique Test E: Enable/Disable messages (no changes)
TEST(GSTATE_Fixture, EnableDisableMessagesSafeCalls)
{
    // Calls should be safe both for real instance and nullptr (no crash)
    gstate_enable_all_messages();
    gstate_disable_all_messages();
    gstate_enable_messages(nullptr); // no-op in non-debug
    gstate_disable_messages(nullptr);
    ASSERT_TRUE(true);
}

// -- Implementations for CREATED state functions (minimal) --
// Provide function bodies matching the FUNC names passed to GSTATE_CREATE_STATE
static void _GSV_ValidInit_state1_s(void) {}
static void _GSV_StateTransition_state1_s(void) {}
static void _GSV_StateTransition_state2_s(void) {}
static void _GSV_ClearQueue_state1_s(void) {}
static void _GSV_IsStateCheck_state1_s(void) {}
static void _GSV_IsStateCheck_state2_s(void) {}
static void _GSV_Priority_state1_s(void) {}
static void _GSV_Priority_state2_s(void) {}
static void _GSV_Priority_state3_s(void) {}
static void _GSV_NoTransition_state1_s(void) {}
static void _GSV_NoTransition_fake_s(void) {}
static void _GSV_StateFunctionExecution_state1_s(void) { GSV_StateFunctionExecution_called = true; }
static void _GSV_ProcessCycle_stateA_s(void) {}
static void _GSV_ProcessCycle_stateB_s(void) {}
static void _GSV_StateDriven_state1_s(void) { gstate_request_state(&GSTATE_SUITE_StateDriven_gstate, &GSV_StateDriven_state2_s); }
static void _GSV_StateDriven_state2_s(void) {}
static void _GSV_EmptyQueue_state1_s(void) { GSV_EmptyQueue_exec_count++; }
static void _GSV_Termination_state1_s(void) {}
static void _GSV_Termination_state2_s(void) {}
static void _GSV_CheckStatesCount_state1_s(void) {}
static void _GSV_Dedup_stateA_s(void) {}
static void _GSV_Dedup_stateB_s(void) {}
static void _GSV_RequestNull_state1_s(void) {}
static void _GSV_RequestSame_state1_s(void) {}
static void _GSV_Reset_state1_s(void) {}
static void _GSV_Reset_state2_s(void) {}

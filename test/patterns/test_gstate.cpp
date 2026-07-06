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
GSTATE_CREATE_LIST(GSTATE_SUITE_ValidInit_states, &GSV_ValidInit_state1_s);
GSTATE_CREATE(GSTATE_SUITE_ValidInit_gstate)
void _GSV_ValidInit_state1_s() {}
TEST(GSTATE_Fixture, ValidInit)
{
    ASSERT_TRUE(gstate_init(&GSTATE_SUITE_ValidInit_gstate, GSTATE_SUITE_ValidInit_states, __arr_len(GSTATE_SUITE_ValidInit_states)));
    ASSERT_TRUE(GSTATE_SUITE_ValidInit_gstate._initialized);
    ASSERT_EQ(GSTATE_SUITE_ValidInit_gstate._curr_state->state_f, GSV_ValidInit_state1_s.state_f);
}

// Test 2: Initialization with NULL GSTATE pointer
TEST(GSTATE_Fixture, InitNullGState)
{
    ASSERT_FALSE(gstate_init(nullptr, nullptr, 0));
    // Expect no crash or assertion (if debug enabled)
}

// Test 3: State transition with queued request
GSTATE_CREATE_STATE(GSV_StateTransition_state1_s, _GSV_StateTransition_state1_s, 0);
GSTATE_CREATE_STATE(GSV_StateTransition_state2_s, _GSV_StateTransition_state2_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_StateTransition_states, &GSV_StateTransition_state1_s, &GSV_StateTransition_state2_s);
GSTATE_CREATE(GSTATE_SUITE_StateTransition_gstate)
void _GSV_StateTransition_state1_s() {}
void _GSV_StateTransition_state2_s() {}
TEST(GSTATE_Fixture, StateTransition)
{
    ASSERT_TRUE(gstate_init(&GSTATE_SUITE_StateTransition_gstate, GSTATE_SUITE_StateTransition_states, __arr_len(GSTATE_SUITE_StateTransition_states)));
    gstate_request_state(&GSTATE_SUITE_StateTransition_gstate, &GSV_StateTransition_state2_s);
    gstate_process(&GSTATE_SUITE_StateTransition_gstate);
    ASSERT_TRUE(gstate_is_state(&GSTATE_SUITE_StateTransition_gstate, &GSV_StateTransition_state2_s));
}

// Test 4: Clear queue
GSTATE_CREATE_STATE(GSV_ClearQueue_state1_s, _GSV_ClearQueue_state1_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_ClearQueue_states, &GSV_ClearQueue_state1_s);
GSTATE_CREATE(GSTATE_SUITE_ClearQueue_gstate)
void _GSV_ClearQueue_state1_s() {}
TEST(GSTATE_Fixture, ClearQueue)
{
    ASSERT_TRUE(gstate_init(&GSTATE_SUITE_ClearQueue_gstate, GSTATE_SUITE_ClearQueue_states, __arr_len(GSTATE_SUITE_ClearQueue_states)));
    gstate_request_state(&GSTATE_SUITE_ClearQueue_gstate, &GSV_ClearQueue_state1_s);
    gstate_clear(&GSTATE_SUITE_ClearQueue_gstate);
    ASSERT_EQ(GSTATE_SUITE_ClearQueue_gstate._queue_cnt, 0);
}

// Test 5: Check current state
GSTATE_CREATE_STATE(GSV_IsStateCheck_state1_s, _GSV_IsStateCheck_state1_s, 0);
GSTATE_CREATE_STATE(GSV_IsStateCheck_state2_s, _GSV_IsStateCheck_state2_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_IsStateCheck_states, &GSV_IsStateCheck_state1_s, &GSV_IsStateCheck_state2_s);
GSTATE_CREATE(GSTATE_SUITE_IsStateCheck_gstate)
void _GSV_IsStateCheck_state1_s() {}
void _GSV_IsStateCheck_state2_s() {}
TEST(GSTATE_Fixture, IsStateCheck)
{
    ASSERT_TRUE(gstate_init(&GSTATE_SUITE_IsStateCheck_gstate, GSTATE_SUITE_IsStateCheck_states, __arr_len(GSTATE_SUITE_IsStateCheck_states)));
    ASSERT_TRUE(gstate_is_state(&GSTATE_SUITE_IsStateCheck_gstate, &GSV_IsStateCheck_state1_s));
    ASSERT_FALSE(gstate_is_state(&GSTATE_SUITE_IsStateCheck_gstate, &GSV_IsStateCheck_state2_s));
}

// Test 6: PrioritySelection
GSTATE_CREATE_STATE(GSV_Priority_state1_s, _GSV_Priority_state1_s, 0);
GSTATE_CREATE_STATE(GSV_Priority_state2_s, _GSV_Priority_state2_s, 2);
GSTATE_CREATE_STATE(GSV_Priority_state3_s, _GSV_Priority_state3_s, 1);
GSTATE_CREATE_LIST(GSTATE_SUITE_Priority_states, &GSV_Priority_state1_s, &GSV_Priority_state2_s, &GSV_Priority_state3_s);
GSTATE_CREATE(GSTATE_SUITE_Priority_gstate)
void _GSV_Priority_state1_s() {}
void _GSV_Priority_state2_s() {}
void _GSV_Priority_state3_s() {}
TEST(GSTATE_Fixture, PrioritySelection)
{
    ASSERT_TRUE(gstate_init(&GSTATE_SUITE_Priority_gstate, GSTATE_SUITE_Priority_states, __arr_len(GSTATE_SUITE_Priority_states)));
    gstate_request_state(&GSTATE_SUITE_Priority_gstate, &GSV_Priority_state2_s);
    gstate_request_state(&GSTATE_SUITE_Priority_gstate, &GSV_Priority_state3_s);
    gstate_process(&GSTATE_SUITE_Priority_gstate);
    ASSERT_TRUE(gstate_is_state(&GSTATE_SUITE_Priority_gstate, &GSV_Priority_state2_s));
    ASSERT_EQ(GSTATE_SUITE_Priority_gstate._queue_cnt, 1);
    gstate_process(&GSTATE_SUITE_Priority_gstate);
    ASSERT_TRUE(gstate_is_state(&GSTATE_SUITE_Priority_gstate, &GSV_Priority_state3_s));
    ASSERT_EQ(GSTATE_SUITE_Priority_gstate._queue_cnt, 0);
}

// Test 7: NoTransition
GSTATE_CREATE_STATE(GSV_NoTransition_state1_s, _GSV_NoTransition_state1_s, 0);
GSTATE_CREATE_STATE(GSV_NoTransition_fake_s, _GSV_NoTransition_fake_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_NoTransition_states, &GSV_NoTransition_state1_s);
GSTATE_CREATE(GSTATE_SUITE_NoTransition_gstate)
void _GSV_NoTransition_state1_s() {}
void _GSV_NoTransition_fake_s() {}
TEST(GSTATE_Fixture, NoTransition)
{
    ASSERT_TRUE(gstate_init(&GSTATE_SUITE_NoTransition_gstate, GSTATE_SUITE_NoTransition_states, __arr_len(GSTATE_SUITE_NoTransition_states)));
    gstate_request_state(&GSTATE_SUITE_NoTransition_gstate, &GSV_NoTransition_fake_s);
    gstate_process(&GSTATE_SUITE_NoTransition_gstate);
    ASSERT_TRUE(gstate_is_state(&GSTATE_SUITE_NoTransition_gstate, &GSV_NoTransition_state1_s));
}

// Test 8: StateFunctionExecution
static bool GSV_StateFunctionExecution_called = false;
GSTATE_CREATE_STATE(GSV_StateFunctionExecution_state1_s, _GSV_StateFunctionExecution_state1_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_StateFunctionExecution_states, &GSV_StateFunctionExecution_state1_s);
GSTATE_CREATE(GSTATE_SUITE_StateFunctionExecution_gstate)
void _GSV_StateFunctionExecution_state1_s()
{
    GSV_StateFunctionExecution_called = true;
}
TEST(GSTATE_Fixture, StateFunctionExecution)
{
    GSV_StateFunctionExecution_called = false;
    ASSERT_TRUE(gstate_init(&GSTATE_SUITE_StateFunctionExecution_gstate, GSTATE_SUITE_StateFunctionExecution_states, __arr_len(GSTATE_SUITE_StateFunctionExecution_states)));
    gstate_process(&GSTATE_SUITE_StateFunctionExecution_gstate);
    ASSERT_TRUE(GSV_StateFunctionExecution_called);
}

// Test 9: ProcessCycle
GSTATE_CREATE_STATE(GSV_ProcessCycle_stateA_s, _GSV_ProcessCycle_stateA_s, 0);
GSTATE_CREATE_STATE(GSV_ProcessCycle_stateB_s, _GSV_ProcessCycle_stateB_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_ProcessCycle_states, &GSV_ProcessCycle_stateA_s, &GSV_ProcessCycle_stateB_s);
GSTATE_CREATE(GSTATE_SUITE_ProcessCycle_gstate)
void _GSV_ProcessCycle_stateA_s() {}
void _GSV_ProcessCycle_stateB_s() {}
TEST(GSTATE_Fixture, ProcessCycle)
{
    ASSERT_TRUE(gstate_init(&GSTATE_SUITE_ProcessCycle_gstate, GSTATE_SUITE_ProcessCycle_states, __arr_len(GSTATE_SUITE_ProcessCycle_states)));
    for (int i = 0; i < 3; ++i) {
        gstate_request_state(&GSTATE_SUITE_ProcessCycle_gstate, &GSV_ProcessCycle_stateB_s);
        gstate_process(&GSTATE_SUITE_ProcessCycle_gstate);
    }
    ASSERT_TRUE(gstate_is_state(&GSTATE_SUITE_ProcessCycle_gstate, &GSV_ProcessCycle_stateB_s));
}

// Test 10: StateDrivenRequests
GSTATE_CREATE_STATE(GSV_StateDriven_state1_s, _GSV_StateDriven_state1_s, 0);
GSTATE_CREATE_STATE(GSV_StateDriven_state2_s, _GSV_StateDriven_state2_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_StateDriven_states, &GSV_StateDriven_state1_s, &GSV_StateDriven_state2_s);
GSTATE_CREATE(GSTATE_SUITE_StateDriven_gstate)
void _GSV_StateDriven_state1_s()
{
    gstate_request_state(&GSTATE_SUITE_StateDriven_gstate, &GSV_StateDriven_state2_s);
}
void _GSV_StateDriven_state2_s() {}
TEST(GSTATE_Fixture, StateDrivenRequests)
{
    ASSERT_TRUE(gstate_init(&GSTATE_SUITE_StateDriven_gstate, GSTATE_SUITE_StateDriven_states, __arr_len(GSTATE_SUITE_StateDriven_states)));
    gstate_process(&GSTATE_SUITE_StateDriven_gstate);
    ASSERT_TRUE(gstate_is_state(&GSTATE_SUITE_StateDriven_gstate, &GSV_StateDriven_state2_s));
}

// Test 11: EmptyQueueProcessing
static int GSV_EmptyQueue_exec_count = 0;
GSTATE_CREATE_STATE(GSV_EmptyQueue_state1_s, _GSV_EmptyQueue_state1_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_EmptyQueue_states, &GSV_EmptyQueue_state1_s);
GSTATE_CREATE(GSTATE_SUITE_EmptyQueue_gstate)
void _GSV_EmptyQueue_state1_s()
{
    GSV_EmptyQueue_exec_count++;
}
TEST(GSTATE_Fixture, EmptyQueueProcessing)
{
    GSV_EmptyQueue_exec_count = 0;
    ASSERT_TRUE(gstate_init(&GSTATE_SUITE_EmptyQueue_gstate, GSTATE_SUITE_EmptyQueue_states, __arr_len(GSTATE_SUITE_EmptyQueue_states)));
    for (int i = 0; i < 5; ++i) {
        gstate_process(&GSTATE_SUITE_EmptyQueue_gstate);
    }
    ASSERT_EQ(GSV_EmptyQueue_exec_count, 5);
}

// Test 12: TerminationCondition
GSTATE_CREATE_STATE(GSV_Termination_state1_s, _GSV_Termination_state1_s, 0);
GSTATE_CREATE_STATE(GSV_Termination_state2_s, _GSV_Termination_state2_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_Termination_states, &GSV_Termination_state1_s, &GSV_Termination_state2_s);
GSTATE_CREATE(GSTATE_SUITE_Termination_gstate)
void _GSV_Termination_state1_s() {}
void _GSV_Termination_state2_s() {}
TEST(GSTATE_Fixture, TerminationCondition)
{
    ASSERT_TRUE(gstate_init(&GSTATE_SUITE_Termination_gstate, GSTATE_SUITE_Termination_states, __arr_len(GSTATE_SUITE_Termination_states)));
    for (int i = 0; i < 5; ++i) {
        gstate_request_state(&GSTATE_SUITE_Termination_gstate, &GSV_Termination_state2_s);
    }
    bool target_reached = false;
    for (int i = 0; i < 10; ++i) {
        gstate_process(&GSTATE_SUITE_Termination_gstate);
        target_reached = target_reached || gstate_is_state(&GSTATE_SUITE_Termination_gstate, &GSV_Termination_state2_s);
    }
    ASSERT_TRUE(target_reached);
    ASSERT_EQ(GSTATE_SUITE_Termination_gstate._queue_cnt, 0);
}

// Test 13: CheckStatesCount
GSTATE_CREATE_STATE(GSV_CheckStatesCount_state1_s, _GSV_CheckStatesCount_state1_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_CheckStatesCount_states, &GSV_CheckStatesCount_state1_s);
GSTATE_CREATE(GSTATE_SUITE_CheckStatesCount_gstate)
void _GSV_CheckStatesCount_state1_s() {}
TEST(GSTATE_Fixture, CheckStatesCount)
{
    ASSERT_TRUE(gstate_init(&GSTATE_SUITE_CheckStatesCount_gstate, GSTATE_SUITE_CheckStatesCount_states, __arr_len(GSTATE_SUITE_CheckStatesCount_states)));
    ASSERT_EQ(GSTATE_SUITE_CheckStatesCount_gstate._states_count, 1u);
}

// Test 14: Deduplication
GSTATE_CREATE_STATE(GSV_Dedup_stateA_s, _GSV_Dedup_stateA_s, 0);
GSTATE_CREATE_STATE(GSV_Dedup_stateB_s, _GSV_Dedup_stateB_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_Dedup_states, &GSV_Dedup_stateA_s, &GSV_Dedup_stateB_s, &GSV_Dedup_stateA_s);
GSTATE_CREATE(GSTATE_SUITE_Dedup_gstate)
void _GSV_Dedup_stateA_s() {}
void _GSV_Dedup_stateB_s() {}
TEST(GSTATE_Fixture, DeduplicationRemovesDuplicatesAndEmpty)
{
    ASSERT_TRUE(gstate_init(&GSTATE_SUITE_Dedup_gstate, GSTATE_SUITE_Dedup_states, __arr_len(GSTATE_SUITE_Dedup_states)));
    ASSERT_EQ(GSTATE_SUITE_Dedup_gstate._states_count, 2);
    ASSERT_TRUE(gstate_is_state(&GSTATE_SUITE_Dedup_gstate, &GSV_Dedup_stateA_s));
}

// Test 15: RequestNullTargetIsIgnored
GSTATE_CREATE_STATE(GSV_RequestNull_state1_s, _GSV_RequestNull_state1_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_RequestNull_states, &GSV_RequestNull_state1_s);
GSTATE_CREATE(GSTATE_SUITE_RequestNull_gstate)
void _GSV_RequestNull_state1_s() {}
TEST(GSTATE_Fixture, RequestNullTargetIsIgnored)
{
    ASSERT_TRUE(gstate_init(&GSTATE_SUITE_RequestNull_gstate, GSTATE_SUITE_RequestNull_states, __arr_len(GSTATE_SUITE_RequestNull_states)));
    ASSERT_EQ(GSTATE_SUITE_RequestNull_gstate._queue_cnt, 0);
    gstate_request_state(&GSTATE_SUITE_RequestNull_gstate, NULL);
    ASSERT_EQ(GSTATE_SUITE_RequestNull_gstate._queue_cnt, 0);
}

// Test 16: RequestingCurrentStateIsIgnored
GSTATE_CREATE_STATE(GSV_RequestSame_state1_s, _GSV_RequestSame_state1_s, 0);
GSTATE_CREATE_LIST(GSTATE_SUITE_RequestSame_states, &GSV_RequestSame_state1_s);
GSTATE_CREATE(GSTATE_SUITE_RequestSame_gstate)
void _GSV_RequestSame_state1_s() {}
TEST(GSTATE_Fixture, RequestingCurrentStateIsIgnored)
{
    ASSERT_TRUE(gstate_init(&GSTATE_SUITE_RequestSame_gstate, GSTATE_SUITE_RequestSame_states, __arr_len(GSTATE_SUITE_RequestSame_states)));
    gstate_request_state(&GSTATE_SUITE_RequestSame_gstate, &GSV_RequestSame_state1_s);
    gstate_process(&GSTATE_SUITE_RequestSame_gstate);
    ASSERT_TRUE(gstate_is_state(&GSTATE_SUITE_RequestSame_gstate, &GSV_RequestSame_state1_s));
    ASSERT_EQ(GSTATE_SUITE_RequestSame_gstate._queue_cnt, 0);
}

// Test 17: Reset behavior
GSTATE_CREATE_STATE(GSV_Reset_state1_s, _GSV_Reset_state1_s, 0);
GSTATE_CREATE_STATE(GSV_Reset_state2_s, _GSV_Reset_state2_s, 1);
GSTATE_CREATE_LIST(GSTATE_SUITE_ResetBehavior_states, NULL, &GSV_Reset_state1_s, NULL, &GSV_Reset_state2_s, NULL);
GSTATE_CREATE(GSTATE_SUITE_ResetBehavior_gstate)
void _GSV_Reset_state1_s() {}
void _GSV_Reset_state2_s() {}
TEST(GSTATE_Fixture, ResetRestoresFirstStateAndClearsQueue)
{
    ASSERT_TRUE(gstate_init(&GSTATE_SUITE_ResetBehavior_gstate, GSTATE_SUITE_ResetBehavior_states, __arr_len(GSTATE_SUITE_ResetBehavior_states)));
    ASSERT_EQ(GSTATE_SUITE_ResetBehavior_gstate._states_count, 2);
    gstate_request_state(&GSTATE_SUITE_ResetBehavior_gstate, &GSV_Reset_state2_s);
    gstate_process(&GSTATE_SUITE_ResetBehavior_gstate);
    ASSERT_TRUE(gstate_is_state(&GSTATE_SUITE_ResetBehavior_gstate, &GSV_Reset_state2_s));
    gstate_reset(&GSTATE_SUITE_ResetBehavior_gstate);
    ASSERT_TRUE(gstate_is_state(&GSTATE_SUITE_ResetBehavior_gstate, &GSV_Reset_state1_s));
    ASSERT_EQ(GSTATE_SUITE_ResetBehavior_gstate._queue_cnt, 0);
}

// Test 18: Enable/Disable messages (no changes)
TEST(GSTATE_Fixture, EnableDisableMessagesSafeCalls)
{
    gstate_enable_all_messages();
    gstate_disable_all_messages();
    gstate_enable_messages(nullptr);
    gstate_disable_messages(nullptr);
    ASSERT_TRUE(true);
}

// Test 19: Check the broken states array
GSTATE_CREATE_STATE(CheckEmptyInitState_gstate1_s, _CheckEmptyInitState_gstate1_s, 0);
GSTATE_CREATE_LIST(CheckEmptyInitState_gstate_list, NULL, &CheckEmptyInitState_gstate1_s, NULL);
GSTATE_CREATE(CheckEmptyInitState_gstate)
void _CheckEmptyInitState_gstate1_s() {}
TEST(GSTATE_Fixture, CheckEmptyInitState)
{
    ASSERT_TRUE(gstate_init(&CheckEmptyInitState_gstate, CheckEmptyInitState_gstate_list, __arr_len(CheckEmptyInitState_gstate_list)));

    ASSERT_TRUE(gstate_is_state(&CheckEmptyInitState_gstate, &CheckEmptyInitState_gstate1_s));

    ASSERT_EQ(CheckEmptyInitState_gstate._states_count, 1);
    ASSERT_EQ(CheckEmptyInitState_gstate._curr_state, &CheckEmptyInitState_gstate1_s);
    ASSERT_EQ(CheckEmptyInitState_gstate._states_list[0], &CheckEmptyInitState_gstate1_s);
}

/* Copyright © 2025 Georgy E. All rights reserved. */

#include "gpid.hpp"

#include <cmath>
#include <algorithm>

#include "glog.h"
#include "gutils.h"


GPID::GPID(
    float maxOut,
    float minOut,
    float inputAlpha,
    float iTauAttack,
    float iTauDecay,
    float dDeadzone,
    float dAwayFactor,
    float dMaxMeasuredVel
):
    _pidKp(0.0f), _pidKi(0.0f), _pidKd(0.0f),
    _inputAlpha(inputAlpha), _iTauAtatck(iTauAttack), _iTauDecay(iTauDecay), 
    _dDeadzone(dDeadzone), _dAwayFactor(dAwayFactor), _dMaxMeasuredVel(dMaxMeasuredVel),
    _minOut(minOut), _maxOut(maxOut),
    _pidIntegral(0.0f), _pidDerivative(0.0f), _pidPrevErr(0.0f), 
    _pidFilteredInput(0.0f), _pidDerOutput(0.0f), _pidOutput(0.0f),
    _pidPrevDerAngle(0.0f), _lastRunTimeUs(0), _lastDerTimeUs(0),
    _debugEnabled(false), _debugDelayMs(0), 
    _debugErr(0.0f), _debugPidOutput(0), _debugKp(0), _debugKi(0), _debugKd(0), 
    _debugCurrent(0), _debugTarget(0)
{
    reset();
}

void GPID::reset()
{
    _pidDerivative = 0.0f;
    _pidIntegral = 0.0f;
    _pidPrevErr = 0.0f;
    _pidDerOutput = 0.0f;
    _pidOutput = 0.0f;

    _lastRunTimeUs = 0;
    _lastDerTimeUs = 0;
    _pidPrevDerAngle = _pidFilteredInput;

    _debugPidOutput = 0;
    _debugKp = 0;
    _debugKi = 0;
    _debugKd = 0;
    _debugCurrent = 0;
    _debugTarget = 0;
}

void GPID::print(
    float err,
    float pidOutput,
    float kp_out,
    float ki_out,
    float kd_out,
    float current,
    float target
) {
    _debugErr = err;
    _debugPidOutput = (int32_t)__proportion(pidOutput, -_maxOut, _maxOut, -100.0f, 100.0f);
    _debugKp = (int32_t)__proportion(kp_out, -_maxOut, _maxOut, -100.0f, 100.0f);
    _debugKi = (int32_t)__proportion(ki_out, -_maxOut, _maxOut, -100.0f, 100.0f);
    _debugKd = (int32_t)__proportion(kd_out, -_maxOut, _maxOut, -100.0f, 100.0f);
    _debugCurrent = current;
    _debugTarget = target;
}

void GPID::show()
{
    if (_debugEnabled) {
        gprintMsgFilter(
            gprint(
                "%lu,%.01f,%d,%d,%d,%d,%u,%u\n",
                (uint32_t)getMicroseconds(),
                _debugErr,
                _debugPidOutput,
                _debugKp,
                _debugKi,
                _debugKd,
                _debugCurrent,
                _debugTarget
            ),
            _debugDelayMs
        );
    }
}

float GPID::update(
    float current,
    float target
) {
    // ===========================
    float dt = 1.0f / 1000.0f;
    uint64_t now_us = getMicroseconds();
    if (now_us > _lastRunTimeUs) {
        dt = (float)(now_us - _lastRunTimeUs) / (float)(1000000.0f);
    }
    _lastRunTimeUs = now_us;
    // ===========================

    _pidFilteredInput = (_inputAlpha * current) + (1.0f - _inputAlpha) * _pidFilteredInput;
    float err = target - _pidFilteredInput;

    // ===========================
    float kd_out = 0.0f;
    if (_pidKd > 1e-6f) {
        kd_out = _pidKd * (err - _pidPrevErr) / dt; // unit/sec
        // if (_pidDerivative > _dMaxMeasuredVel) {
        //     _pidDerivative = _dMaxMeasuredVel;
        // } else if (_pidDerivative < -_dMaxMeasuredVel) {
        //     _pidDerivative = -_dMaxMeasuredVel;
        // }
        const float ALPHA = 1.0f;
        _pidDerOutput = (ALPHA * kd_out) + (1.0f - ALPHA) * _pidDerOutput;
        // if (__abs(_pidDerivative) < _dDeadzone) {
        //     _pidDerivative = 0.0f;
        // }
        kd_out = _pidDerOutput;
    }
    // ===========================


    // ===========================
    float kp_out = _pidKp * err;
    if (__abs(kp_out) > _maxOut) {
        kp_out = kp_out > 0.0f ? _maxOut : -_maxOut;
    }
    // ===========================


    // ===========================
    float ki_out = 0.0f;
    if (_pidKi > 1e-6f) {
        ki_out = _pidKi * _pidIntegral;
        float unsat_out = kp_out + ki_out + kd_out;
        // // anti-windup
        bool saturated_pos = (unsat_out > _maxOut && err > 0.0f);
        bool saturated_neg = (unsat_out < -_maxOut && err < 0.0f);
        if (!saturated_pos && !saturated_neg) {
            // интегрирование
            float integral = err * dt;
            // проверка на движение от цели
            if (sign(err) * sign(_pidIntegral) > 0.0f) {
                _pidIntegral = _pidIntegral * (1.0f - _iTauAtatck) + integral;
            } else {
                _pidIntegral = _pidIntegral * (1.0f - _iTauDecay) + integral;
            }
            // проверка предела
            if (__abs(_pidIntegral) > _maxOut) {
                _pidIntegral = _pidIntegral > 0.0f ? _maxOut : -_maxOut;
            }
        }
        // ===========================
        
        // ===========================
        // recompute ki_out after integrator update
        ki_out = _pidKi * _pidIntegral;
        // ===========================
    }
    
    // ===========================
    float out = kp_out + ki_out + kd_out;
    if (out > 0.0f) {
        out  = __proportion(out, 0.0f, _maxOut, _minOut, _maxOut);
    } else {
        out  = __proportion(out, -_maxOut, 0.0f, -_maxOut, -_minOut);
    }
    if (__abs(out) > _maxOut) {
        out = out > 0.0f ? _maxOut : -_maxOut;
    }
    _pidOutput = out;
    // =============================
    
    print(
        err,
        _pidOutput,
        kp_out,
        ki_out,
        kd_out,
        _pidFilteredInput,
        target
    );

    _pidPrevErr = err;

    return _pidOutput;
}
/* Copyright © 2025 Georgy E. All rights reserved. */

#include "gpid.hpp"

#include "glog.h"
#include "gutils.h"

// #include <limits>


static const gq15_16_t GQ_ZERO = 0;
static const gq15_16_t GQ_ONE = GQ15_16_SCALE;
static const gq15_16_t GQ_DEFAULT_DT = 7;


static gq15_16_t _gq15_16_from_microseconds(uint32_t microseconds)
{
    int64_t raw = ((int64_t)microseconds * GQ15_16_SCALE + 500000) / 1000000;
    if (raw < 1) {
        return 1;
    }
    if (raw > INT32_MAX) {
        return INT32_MAX;
    }
    return (gq15_16_t)raw;
}

static int _gq15_16_sign(gq15_16_t value)
{
    return value > 0 ? 1 : (value < 0 ? -1 : 0);
}

static gq15_16_t _gq15_16_abs(gq15_16_t value)
{
    return value == INT32_MIN ? INT32_MAX : (value < 0 ? -value : value);
}

static gq15_16_t _gq15_16_clamp(gq15_16_t value, gq15_16_t limit)
{
    if (value > limit) {
        return limit;
    }
    if (value < -limit) {
        return -limit;
    }
    return value;
}


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
    _pidKp(GQ_ZERO), _pidKi(GQ_ZERO), _pidKd(GQ_ZERO),
    _inputAlpha(gq15_16_from_float(inputAlpha)), _iTauAtatck(gq15_16_from_float(iTauAttack)), _iTauDecay(gq15_16_from_float(iTauDecay)),
    _dDeadzone(gq15_16_from_float(dDeadzone)), _dAwayFactor(gq15_16_from_float(dAwayFactor)), _dMaxMeasuredVel(gq15_16_from_float(dMaxMeasuredVel)),
    _minOut(gq15_16_from_float(minOut)), _maxOut(gq15_16_from_float(maxOut)),
    _pidIntegral(GQ_ZERO), _pidDerivative(GQ_ZERO), _pidPrevErr(GQ_ZERO),
    _pidFilteredInput(GQ_ZERO), _pidDerOutput(GQ_ZERO), _pidOutput(GQ_ZERO),
    _pidPrevDerAngle(GQ_ZERO), _lastRunTimeUs(0), _lastDerTimeUs(0),
    _debugEnabled(false), _debugDelayMs(0), 
    _debugErr(GQ_ZERO), _debugPidOutput(0), _debugKp(0), _debugKi(0), _debugKd(0),
    _debugCurrent(0), _debugTarget(0)
{
    reset();
}

void GPID::setGains(float pidKp, float pidKi, float pidKd)
{
    _pidKp = gq15_16_from_float(pidKp);
    _pidKi = gq15_16_from_float(pidKi);
    _pidKd = gq15_16_from_float(pidKd);
}

void GPID::setOutputLimits(float minOut, float maxOut)
{
    _minOut = gq15_16_from_float(minOut);
    _maxOut = gq15_16_from_float(maxOut);
}

void GPID::reset()
{
    _pidDerivative = GQ_ZERO;
    _pidIntegral = GQ_ZERO;
    _pidPrevErr = GQ_ZERO;
    _pidDerOutput = GQ_ZERO;
    _pidOutput = GQ_ZERO;

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
    gq15_16_t err,
    gq15_16_t pidOutput,
    gq15_16_t kp_out,
    gq15_16_t ki_out,
    gq15_16_t kd_out,
    gq15_16_t current,
    gq15_16_t target
) {
    _debugErr = err;
    _debugPidOutput = _maxOut ? (int16_t)gq15_16_to_int(gq15_16_div(gq15_16_mul(pidOutput, gq15_16_from_int(100)), _maxOut)) : 0;
    _debugKp = _maxOut ? (int16_t)gq15_16_to_int(gq15_16_div(gq15_16_mul(kp_out, gq15_16_from_int(100)), _maxOut)) : 0;
    _debugKi = _maxOut ? (int16_t)gq15_16_to_int(gq15_16_div(gq15_16_mul(ki_out, gq15_16_from_int(100)), _maxOut)) : 0;
    _debugKd = _maxOut ? (int16_t)gq15_16_to_int(gq15_16_div(gq15_16_mul(kd_out, gq15_16_from_int(100)), _maxOut)) : 0;
    _debugCurrent = (uint16_t)gq15_16_to_int(current);
    _debugTarget = (uint16_t)gq15_16_to_int(target);
}

void GPID::show()
{
    if (_debugEnabled) {
        gprintMsgFilter(
            gprint(
                "%lu,%d.%d,%d,%d,%d,%d,%u,%u\n",
                (uint32_t)getMicroseconds(),
                (int)gq15_16_to_int(_debugErr), (int)gq15_16_to_int(gq15_16_mul(_gq15_16_abs(_debugErr), gq15_16_from_int(10))),
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

float GPID::update_f(
    float current,
    float target
) {
    return gq15_16_to_float(update_q15_16(gq15_16_from_float(current), gq15_16_from_float(target)));
}

gq15_16_t GPID::update_q15_16(
    gq15_16_t current,
    gq15_16_t target
) {
    gq15_16_t dt = GQ_DEFAULT_DT;
    uint64_t now_us = getMicroseconds();
    if (now_us > _lastRunTimeUs) {
        uint32_t delta_us = (uint32_t)(now_us - _lastRunTimeUs);
        dt = _gq15_16_from_microseconds(delta_us);
    }
    _lastRunTimeUs = now_us;

    _pidFilteredInput = gq15_16_add(
        gq15_16_mul(_inputAlpha, current),
        gq15_16_mul(gq15_16_sub(GQ_ONE, _inputAlpha), _pidFilteredInput)
    );
    gq15_16_t err = gq15_16_sub(target, _pidFilteredInput);

    gq15_16_t kd_out = GQ_ZERO;
    if (_pidKd > GQ_ZERO) {
        kd_out = gq15_16_div(gq15_16_mul(_pidKd, gq15_16_sub(err, _pidPrevErr)), dt);
        _pidDerOutput = kd_out;
        kd_out = _pidDerOutput;
    }

    gq15_16_t kp_out = _gq15_16_clamp(gq15_16_mul(_pidKp, err), _maxOut);

    gq15_16_t ki_out = GQ_ZERO;
    if (_pidKi > GQ_ZERO) {
        ki_out = gq15_16_mul(_pidKi, _pidIntegral);
        gq15_16_t unsat_out = gq15_16_add(gq15_16_add(kp_out, ki_out), kd_out);
        bool saturated_pos = (unsat_out > _maxOut && err > GQ_ZERO);
        bool saturated_neg = (unsat_out < -_maxOut && err < GQ_ZERO);
        if (!saturated_pos && !saturated_neg) {
            gq15_16_t integral = gq15_16_mul(err, dt);
            if (_gq15_16_sign(err) * _gq15_16_sign(_pidIntegral) > 0) {
                _pidIntegral = gq15_16_add(gq15_16_mul(_pidIntegral, gq15_16_sub(GQ_ONE, _iTauAtatck)), integral);
            } else {
                _pidIntegral = gq15_16_add(gq15_16_mul(_pidIntegral, gq15_16_sub(GQ_ONE, _iTauDecay)), integral);
            }
            _pidIntegral = _gq15_16_clamp(_pidIntegral, _maxOut);
        }
        ki_out = gq15_16_mul(_pidKi, _pidIntegral);
    }

    gq15_16_t out = gq15_16_add(gq15_16_add(kp_out, ki_out), kd_out);
    if (_maxOut <= GQ_ZERO) {
        out = GQ_ZERO;
    } else if (out > GQ_ZERO) {
        out = gq15_16_add(_minOut, gq15_16_div(gq15_16_mul(out, gq15_16_sub(_maxOut, _minOut)), _maxOut));
    } else if (out < GQ_ZERO) {
        out = -gq15_16_add(_minOut, gq15_16_div(gq15_16_mul(_gq15_16_abs(out), gq15_16_sub(_maxOut, _minOut)), _maxOut));
    }
    _pidOutput = _gq15_16_clamp(out, _maxOut);

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
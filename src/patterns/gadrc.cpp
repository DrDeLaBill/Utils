/* Copyright © 2026 Georgy E. All rights reserved. */

#include "gadrc.hpp"
#include "gutils.h"

static const gq15_16_t GQ_ZERO = 0;

static gq15_16_t _gq15_16_clamp(gq15_16_t value, gq15_16_t limit)
{
    if (value > limit) return limit;
    if (value < -limit) return -limit;
    return value;
}

GADRC::GADRC(float maxOut, float minOut) :
    _b0(GQ_ZERO), _b0_inv(GQ_ZERO),
    _beta1_dt(GQ_ZERO), _beta2_dt(GQ_ZERO), _beta3_dt(GQ_ZERO),
    _kp(GQ_ZERO), _kd(GQ_ZERO),
    _z1(GQ_ZERO), _z2(GQ_ZERO), _z3(GQ_ZERO), _u_prev(GQ_ZERO),
    _saturated(false), _lastRunTimeUs(0)
{
    setOutputLimits(minOut, maxOut);
}

void GADRC::setParameters(float wc, float wo, float b0, float dt, int order)
{
    if (dt <= 0.0f || b0 == 0.0f) {
        return;
    }

    _b0 = gq15_16_from_float(b0);
    _b0_inv = gq15_16_from_float(1.0f / b0);

    if (order == 1) {
        _beta1_dt = gq15_16_from_float(2.0f * wo * dt);
        _beta2_dt = gq15_16_from_float(wo * wo * dt);
        _beta3_dt = GQ_ZERO;

        _kp = gq15_16_from_float(wc);
        _kd = GQ_ZERO;
    } else {
        _beta1_dt = gq15_16_from_float(3.0f * wo * dt);
        _beta2_dt = gq15_16_from_float(3.0f * wo * wo * dt);
        _beta3_dt = gq15_16_from_float(wo * wo * wo * dt);

        _kp = gq15_16_from_float(wc * wc);
        _kd = gq15_16_from_float(2.0f * wc);
    }
}

void GADRC::setOutputLimits(float minOut, float maxOut)
{
    _minOut = gq15_16_from_float(minOut);
    _maxOut = gq15_16_from_float(maxOut);
}

void GADRC::reset()
{
    _z1 = GQ_ZERO;
    _z2 = GQ_ZERO;
    _z3 = GQ_ZERO;
    _u_prev = GQ_ZERO;
    _saturated = false;
    _lastRunTimeUs = getMicroseconds();
}

gq15_16_t GADRC::update_1st_order_q15_16(gq15_16_t current, gq15_16_t target)
{
    gq15_16_t e = gq15_16_sub(_z1, current);
    
    gq15_16_t z1_dot = gq15_16_add(gq15_16_add(_z2, gq15_16_mul(_b0, _u_prev)), gq15_16_mul(_beta1_dt, -e));
    _z1 = gq15_16_add(_z1, z1_dot);
    
    gq15_16_t z2_dot = gq15_16_mul(_beta2_dt, -e);
    _z2 = gq15_16_add(_z2, z2_dot);

    gq15_16_t u0 = gq15_16_mul(_kp, gq15_16_sub(target, _z1));
    gq15_16_t u = gq15_16_mul(gq15_16_sub(u0, _z2), _b0_inv);

    gq15_16_t out = _gq15_16_clamp(u, _maxOut);
    _saturated = (out >= _maxOut || out <= -_maxOut);
    
    _u_prev = out;
    return out;
}

gq15_16_t GADRC::update_2nd_order_q15_16(gq15_16_t current, gq15_16_t target)
{
    gq15_16_t e = gq15_16_sub(_z1, current);

    gq15_16_t z1_dot = gq15_16_add(_z2, gq15_16_mul(_beta1_dt, -e));
    _z1 = gq15_16_add(_z1, z1_dot);

    gq15_16_t z2_dot = gq15_16_add(gq15_16_add(_z3, gq15_16_mul(_b0, _u_prev)), gq15_16_mul(_beta2_dt, -e));
    _z2 = gq15_16_add(_z2, z2_dot);

    gq15_16_t z3_dot = gq15_16_mul(_beta3_dt, -e);
    _z3 = gq15_16_add(_z3, z3_dot);

    gq15_16_t err = gq15_16_sub(target, _z1);
    gq15_16_t u0 = gq15_16_sub(gq15_16_mul(_kp, err), gq15_16_mul(_kd, _z2));
    gq15_16_t u = gq15_16_mul(gq15_16_sub(u0, _z3), _b0_inv);

    gq15_16_t out = _gq15_16_clamp(u, _maxOut);
    _saturated = (out >= _maxOut || out <= -_maxOut);
    
    _u_prev = out;
    return out;
}
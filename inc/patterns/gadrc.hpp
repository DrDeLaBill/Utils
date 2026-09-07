/* Copyright © 2026 Georgy E. All rights reserved. */

#ifndef _GADRC_HPP_
#define _GADRC_HPP_

#include <cstdint>
#include "gq15_16.h"

class GADRC
{
private:
    // System parameters precomputed for the fixed-point ISR.
    gq15_16_t _b0;
    gq15_16_t _b0_inv;

    // ESO coefficients multiplied by dt to avoid overflow.
    gq15_16_t _beta1_dt;
    gq15_16_t _beta2_dt;
    gq15_16_t _beta3_dt;

    // SEF controller coefficients.
    gq15_16_t _kp;
    gq15_16_t _kd;

    // Output limits.
    gq15_16_t _minOut;
    gq15_16_t _maxOut;

    // Extended state observer state.
    gq15_16_t _z1;
    gq15_16_t _z2;
    gq15_16_t _z3;

    gq15_16_t _u_prev;
    bool _saturated;

    uint64_t _lastRunTimeUs;

public:
    GADRC(float maxOut, float minOut = 0.0f);

    // Configure the linear ADRC controller.
    // wc is the controller bandwidth in rad/s.
    // wo is the observer bandwidth in rad/s.
    // b0 is the system gain.
    // dt is the expected sample period in seconds.
    // order is 1 for the speed loop or 2 for the position loop.
    void setParameters(float wc, float wo, float b0, float dt, int order);

    void setOutputLimits(float minOut, float maxOut);
    void reset();

    // Update a first-order ADRC controller.
    gq15_16_t update_1st_order_q15_16(gq15_16_t current, gq15_16_t target);

    // Update a second-order ADRC controller.
    gq15_16_t update_2nd_order_q15_16(gq15_16_t current, gq15_16_t target);

    bool saturated() const { return _saturated; }
};

#endif // _GADRC_HPP_
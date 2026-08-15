/* Copyright © 2025 Georgy E. All rights reserved. */

#ifndef _GPID_HPP_
#define _GPID_HPP_


#include <cstdint>

#include "gq15_16.h"



class GPID
{
private:
    // Filter parameters
    static constexpr gq15_16_t D_DEADZONE = GQ15_16_SCALE / 2;
    static constexpr gq15_16_t MAX_MEASURED_VEL = 1000 * GQ15_16_SCALE;

    // PID coefficients
    gq15_16_t _pidKp;
    gq15_16_t _pidKi;
    gq15_16_t _pidKd;

    // General parameters
    gq15_16_t _inputAlpha;
    gq15_16_t _iTauAtatck;        // integral time constant when moving toward the target
    gq15_16_t _iTauDecay;         // integral time constant when moving away from the target
    gq15_16_t _dDeadzone;         // dead zone for measured velocity
    gq15_16_t _dAwayFactor;       // factor for D when moving away from the target (0 = disable D in this case)
    gq15_16_t _dMaxMeasuredVel;   // limit for measured velocity (to protect against spikes)
    gq15_16_t _minOut;            // Minimum absolute output value (PWM units), to overcome static friction
    gq15_16_t _maxOut;            // Maximum absolute output value (PWM units)

    // State accumulators
    gq15_16_t _pidIntegral;       // integral accumulator of velocity (units * sec)
    gq15_16_t _pidDerivative;     // filtered measured velocity (units/sec)
    gq15_16_t _pidPrevErr;        // previous error to detect sign change
    gq15_16_t _pidFilteredInput;  // for derivative calculation
    gq15_16_t _pidDerOutput;
    gq15_16_t _pidOutput;

    // Derivative calculation
    gq15_16_t _pidPrevDerAngle;   // previous angle for derivative
    uint64_t _lastRunTimeUs;  // timestamp of the last PID update (microseconds)
    uint64_t _lastDerTimeUs;  // timestamp of the last derivative (microseconds)

    // Debugging
    bool _debugEnabled;
    uint32_t _debugDelayMs;
    gq15_16_t _debugErr;
    int16_t _debugPidOutput;
    int16_t _debugKp;
    int16_t _debugKi;
    int16_t _debugKd;
    uint16_t _debugCurrent;
    uint16_t _debugTarget;

public:
    GPID(
        float maxOut,
        float minOut = 0.0f,
        float inputAlpha = 0.0f,
        float iTauAttack = 0.0f,
        float iTauDecay = 0.0f,
        float dDeadzone = D_DEADZONE,
        float dAwayFactor = 0.0f,
        float dMaxMeasuredVel = MAX_MEASURED_VEL
    );

    /**
     * @brief Set PID gains for position and velocity loops
     * @param pidKp Proportional gain for the position loop (P -> target velocity)
     * @param pidKi Integral gain for the velocity loop (I -> PWM)
     * @param pidKd Derivative gain for the velocity loop (D -> PWM)
     */
    void setGains(float pidKp, float pidKi, float pidKd);

    /**
     * @brief Set minimum and maximum output values
     * @param minOut Minimum absolute output value (PWM units), to overcome static friction
     * @param maxOut Maximum absolute output value (PWM units)
     */
    void setOutputLimits(float minOut, float maxOut);

    /**
     * @brief Enable or disable debug logging of internal PID parameters
     * @param enabled If true, detailed PID debug information will be printed
     * @param debugDelayMs Minimum interval between debug messages (ms) to avoid log spam
     */
    void setDebugEnabled(bool enabled, uint32_t debugDelayMs = 0) 
    { 
        _debugEnabled = enabled;
        _debugDelayMs = debugDelayMs;
    }

    /**
     * @brief Reset all controller state (integral, derivative, output)
     */
    void reset();

    /**
     * @brief Update the PID controller through the float compatibility adapter
     * @param current Current signal level
     * @param target Target signal level
     * @return PID controller output (PWM units), positive for one direction, negative for the other
     */
    float update_f(
        float current,
        float target
    );

    /**
     * @brief Update the PID controller with Q15.16 values
     * @param current Current signal level in Q15.16 format
     * @param target Target signal level in Q15.16 format
     * @return PID controller output in Q15.16 format
     */
    gq15_16_t update_q15_16(
        gq15_16_t current,
        gq15_16_t target
    );

    void show();

private:
    void print(
        gq15_16_t err,
        gq15_16_t pidOutput,
        gq15_16_t kp_out,
        gq15_16_t ki_out,
        gq15_16_t kd_out,
        gq15_16_t current,
        gq15_16_t target
    );
};

#endif // #ifndef _GPID_HPP_
/* Copyright © 2025 Georgy E. All rights reserved. */

#ifndef _GPID_HPP_
#define _GPID_HPP_


#include <cstdint>



class GPID
{
private:
    // Filter parameters
    static constexpr float D_DEADZONE = 0.5f;
    static constexpr float MAX_MEASURED_VEL = 1000.0f;

    // PID coefficients
    float _pidKp;
    float _pidKi;
    float _pidKd;

    // General parameters
    float _inputAlpha;
    float _iTauAtatck;        // integral time constant when moving toward the target
    float _iTauDecay;         // integral time constant when moving away from the target
    float _dDeadzone;         // dead zone for measured velocity
    float _dAwayFactor;       // factor for D when moving away from the target (0 = disable D in this case)
    float _dMaxMeasuredVel;   // limit for measured velocity (to protect against spikes)
    float _minOut;            // Minimum absolute output value (PWM units), to overcome static friction
    float _maxOut;            // Maximum absolute output value (PWM units)

    // State accumulators
    float _pidIntegral;       // integral accumulator of velocity (units * sec)
    float _pidDerivative;     // filtered measured velocity (units/sec)
    float _pidPrevErr;        // previous error to detect sign change
    float _pidFilteredInput;  // for derivative calculation
    float _pidDerOutput;
    float _pidOutput;

    // Derivative calculation
    float _pidPrevDerAngle;   // previous angle for derivative
    uint64_t _lastRunTimeUs;  // timestamp of the last PID update (microseconds)
    uint64_t _lastDerTimeUs;  // timestamp of the last derivative (microseconds)

    // Debugging
    bool _debugEnabled;
    uint32_t _debugDelayMs;
    float _debugErr;
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
    void setGains(float pidKp, float pidKi, float pidKd) 
    {
        _pidKp = pidKp;
        _pidKi = pidKi;
        _pidKd = pidKd;
    }

    /**
     * @brief Set minimum and maximum output values
     * @param minOut Minimum absolute output value (PWM units), to overcome static friction
     * @param maxOut Maximum absolute output value (PWM units)
     */
    void setOutputLimits(float minOut, float maxOut) 
    {
        _minOut = minOut;
        _maxOut = maxOut;
    }

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
     * @brief Update the PID controller with current position and target
     * @param current Current signal level
     * @param target Target signal level
     * @return PID controller output (PWM units), positive for one direction, negative for the other
     */
    float update(
        float current,
        float target
    );

    void show();

private:
    float sign(float value) { return value > 0.0f ? 1.0f : -1.0f; }

    void print(
        float err,
        float pidOutput,
        float kp_out,
        float ki_out,
        float kd_out,
        float current,
        float target
    );
};

#endif // #ifndef _GPID_HPP_
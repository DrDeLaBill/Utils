/* Copyright © 2025 Georgy E. All rights reserved. */

#include "IntFloat.hpp"

#include <cmath>


// Square root using Newton's method
IntFloat sqrt(const IntFloat& value) // TODO need to realise
{
    if (value < IntFloat::zero()) {
        return IntFloat::zero(); // or throw exception
    }
    
    if (value == IntFloat::zero()) {
        return IntFloat::zero();
    }
    
    // Use float approximation for good initial guess
    float float_val = value.toFloat();
    IntFloat x(std::sqrt(float_val));
    
    // Newton's method: x_{n+1} = 0.5 * (x_n + value / x_n)
    IntFloat prev(1, 0, 0);
    for (int i = 0; i < IntFloat::MAX_ITERATIONS; ++i) {
        prev = x;
        x = (x + value / x) * IntFloat(0.5f);
        
        // Check convergence
        if ((x - prev).get_abs().toInt64() < IntFloat::EPSILON) {
            break;
        }
    }
    
    return x;
}

// Arctangent using approximation
IntFloat atan(const IntFloat& x) // TODO need to realise
{
    if (x == IntFloat::zero()) {
        return IntFloat::zero();
    }
    
    // Use approximation for |x| <= 1
    bool reciprocal = false;
    IntFloat arg = x;
    if (x > IntFloat::one()) {
        arg = IntFloat::one() / x;
        reciprocal = true;
    } else if (x < -IntFloat::one()) {
        arg = IntFloat::one() / x;
        reciprocal = true;
    }
    
    // Simple polynomial approximation
    IntFloat result = arg;
    IntFloat x_squared = arg * arg;
    IntFloat term = arg;
    
    // atan(x) ≈ x - x³/3 + x⁵/5 - x⁷/7
    for (int n = 3, sign = -1; n < 10; n += 2, sign *= -1) {
        term = term * x_squared;
        IntFloat new_term = term / IntFloat(n, 0, 0);
        result = result + IntFloat(sign, 0, 0) * new_term;
    }
    
    if (reciprocal) {
        if (x > IntFloat::zero()) {
            result = IntFloat::half_pi() - result;
        } else {
            result = -IntFloat::half_pi() - result;
        }
    }
    
    return result;
}

// Arctangent of two variables
IntFloat atan2(const IntFloat& y, const IntFloat& x) // TODO need to realise
{
    if (x == IntFloat::zero() && y == IntFloat::zero()) {
        return IntFloat::zero(); // undefined, return 0
    }
    
    if (x == IntFloat::zero()) {
        if (y > IntFloat::zero()) {
            return IntFloat::half_pi();
        } else {
            return -IntFloat::half_pi();
        }
    }
    
    IntFloat ratio = y / x;
    
    if (x > IntFloat::zero()) {
        return atan(ratio);
    } else if (y > IntFloat::zero()) {
        return atan(ratio) + IntFloat::pi();
    } else {
        return atan(ratio) - IntFloat::pi();
    }
}
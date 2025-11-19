/* Copyright © 2025 Georgy E. All rights reserved. */

#include <gtest/gtest.h>

#include <cmath>
#include <limits>

#include "IntFloat.hpp"


static const float TEST_EPSILON = 1e-6f;


// Basic construction and conversion tests
TEST(UtilsFixture, test_construction_from_float)
{
    IntFloat f1(3.14f);
    EXPECT_NEAR(3.14f, f1.toFloat(), 1e-6f);
    
    IntFloat f2(-2.5f);
    EXPECT_NEAR(-2.5f, f2.toFloat(), 1e-6f);
    
    IntFloat f3(0.0f);
    EXPECT_NEAR(0.0f, f3.toFloat(), 1e-6f);
}

TEST(UtilsFixture, test_construction_from_parts)
{
    IntFloat f1(3, 140000000, IntFloat::FRACT_RESOLUTION);
    EXPECT_NEAR(3.14f, f1.toFloat(), 1e-6f);
    
    IntFloat f2(-2, 500000000, IntFloat::FRACT_RESOLUTION);
    EXPECT_NEAR(-2.5f, f2.toFloat(), 1e-6f);
    
    IntFloat f3(0, 0, 0);
    EXPECT_NEAR(0.0f, f3.toFloat(), 1e-6f);
}

// Arithmetic operations tests
TEST(UtilsFixture, test_addition_operation)
{
    IntFloat a(2.5f);
    IntFloat b(1.25f);
    IntFloat result = a + b;
    EXPECT_NEAR(3.75f, result.toFloat(), 1e-6f);
    
    // Test with negative numbers
    IntFloat c(-1.5f);
    IntFloat d(3.0f);
    IntFloat result2 = c + d;
    EXPECT_NEAR(1.5f, result2.toFloat(), 1e-6f);
}

TEST(UtilsFixture, test_subtraction_operation)
{
    IntFloat a(5.75f);
    IntFloat b(2.25f);
    IntFloat result = a - b;
    EXPECT_NEAR(3.5f, result.toFloat(), 1e-6f);
    
    // Test with negative result
    IntFloat c(1.0f);
    IntFloat d(3.0f);
    IntFloat result2 = c - d;
    EXPECT_NEAR(-2.0f, result2.toFloat(), 1e-6f);
}

TEST(UtilsFixture, test_multiplication_operation)
{
    IntFloat a(2.5f);
    IntFloat b(4.0f);
    IntFloat result = a * b;
    EXPECT_NEAR(10.0f, result.toFloat(), 1e-6f);
    
    // Test with fractions
    IntFloat c(0.5f);
    IntFloat d(0.5f);
    IntFloat result2 = c * d;
    EXPECT_NEAR(0.25f, result2.toFloat(), 1e-6f);

    IntFloat e(1, 1, IntFloat::FRACT_RESOLUTION / 10);
    IntFloat f(8, 1, IntFloat::FRACT_RESOLUTION / 10);
    IntFloat result3 = e * f;
    EXPECT_NEAR(8.000000900000001f, result3.toFloat(), 1e-6f);

    IntFloat g(1, 1, IntFloat::FRACT_RESOLUTION);
    IntFloat h(8, 1, IntFloat::FRACT_RESOLUTION);
    IntFloat result4 = g * h;
    EXPECT_NEAR(8.0f, result4.toFloat(), 1e-6f);

    IntFloat i = 1395.0000f;
    IntFloat j = 0.000061f;
    IntFloat result5 = i * j;
    printf("%.6f %.6f\n", 0.085095f, result5.toFloat());
    EXPECT_NEAR(0.085095f, result5.toFloat(), 15e-7f);
}

TEST(UtilsFixture, test_division_operation)
{
    IntFloat a(10.0f);
    IntFloat b(4.0f);
    IntFloat result = a / b;
    EXPECT_NEAR(2.5f, result.toFloat(), 1e-6f);
    
    // Test with fractions
    IntFloat c(1.0f);
    IntFloat d(8.0f);
    IntFloat result2 = c / d;
    EXPECT_NEAR(0.125f, result2.toFloat(), 1e-6f);
}

TEST(UtilsFixture, test_unary_minus_operation)
{
    IntFloat a(3.14f);
    IntFloat result = -a;
    EXPECT_NEAR(-3.14f, result.toFloat(), 1e-6f);
    
    IntFloat b(-2.5f);
    IntFloat result2 = -b;
    EXPECT_NEAR(2.5f, result2.toFloat(), 1e-6f);
    
    IntFloat c(0.0f);
    IntFloat result3 = -c;
    EXPECT_NEAR(0.0f, result3.toFloat(), 1e-6f);
}

// Complex multi-operation tests
TEST(UtilsFixture, test_complex_arithmetic_sequence)
{
    IntFloat a(10.0f);
    IntFloat b(2.0f);
    IntFloat c(3.0f);
    
    // (a + b) * c - a / b
    IntFloat result = (a + b) * c - a / b;
    float expected = (10.0f + 2.0f) * 3.0f - 10.0f / 2.0f;
    EXPECT_NEAR(expected, result.toFloat(), 1e-5f);
}

TEST(UtilsFixture, test_chained_operations)
{
    IntFloat value(2.0f);
    
    // value = (((value * 2) + 1) / 3) - 0.5
    IntFloat result = ((value * IntFloat(2.0f)) + IntFloat(1.0f)) / IntFloat(3.0f) - IntFloat(0.5f);
    float expected = (((2.0f * 2.0f) + 1.0f) / 3.0f) - 0.5f;
    EXPECT_NEAR(expected, result.toFloat(), 1e-6f);
}

// Comparison operators tests
TEST(UtilsFixture, test_comparison_operators)
{
    IntFloat a(2.5f);
    IntFloat b(2.5f);
    IntFloat c(3.0f);
    IntFloat d(2.0f);
    
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
    
    EXPECT_TRUE(a != c);
    EXPECT_FALSE(a != b);
    
    EXPECT_TRUE(d < a);
    EXPECT_TRUE(a < c);
    EXPECT_FALSE(c < a);
    
    EXPECT_TRUE(c > a);
    EXPECT_TRUE(a > d);
    EXPECT_FALSE(d > a);
    
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(a <= c);
    EXPECT_FALSE(c <= a);
    
    EXPECT_TRUE(a >= b);
    EXPECT_TRUE(c >= a);
    EXPECT_FALSE(d >= a);
}

// Constants and utility functions tests
TEST(UtilsFixture, test_constants)
{
    EXPECT_NEAR(3.141592654f, IntFloat::pi().toFloat(), 1e-6f);
    EXPECT_NEAR(1.570796327f, IntFloat::half_pi().toFloat(), 1e-6f);
    EXPECT_NEAR(0.785398163f, IntFloat::quarter_pi().toFloat(), 1e-6f);
    EXPECT_NEAR(9.80665f, IntFloat::gravity_earth().toFloat(), 1e-6f);
}

TEST(UtilsFixture, test_absolute_value)
{
    IntFloat a(3.14f);
    IntFloat b(-3.14f);
    IntFloat c(0.0f);
    
    EXPECT_NEAR(3.14f, a.get_abs().toFloat(), 1e-6f);
    EXPECT_NEAR(3.14f, b.get_abs().toFloat(), 1e-6f);
    EXPECT_NEAR(0.0f, c.get_abs().toFloat(), 1e-6f);
}

TEST(UtilsFixture, test_is_zero)
{
    IntFloat a(0.0f);
    IntFloat b(0.000000001f); // Very small but not zero
    IntFloat c(1.0f);
    
    EXPECT_TRUE(a.isZero());
    EXPECT_TRUE(b.isZero()); // Should be true due to EPSILON
    EXPECT_FALSE(c.isZero());
}

// Mathematical functions comparison tests
TEST(UtilsFixture, test_sqrt_comparison)
{
    // Test values for square root
    float test_values[] = {0.0f, 1.0f, 2.0f, 4.0f, 9.0f, 16.0f, 25.0f, 0.5f, 0.25f};
    
    for (float val : test_values) {
        IntFloat int_float_val(val);
        
        // Simple sqrt implementation for IntFloat (you need to implement this)
        // This is a placeholder - you'll need to implement actual sqrt for IntFloat
        float std_sqrt = std::sqrt(val);
        
        // For now, just test the conversion back and forth
        // Once you implement sqrt for IntFloat, replace this with actual comparison
        float int_float_sqrt = int_float_val.toFloat(); // Placeholder
        
        // For positive values, sqrt should be reasonably close
        if (val >= 0) {
            // This test will fail until you implement sqrt - it's here as a template
            // EXPECT_NEAR(std_sqrt, int_float_sqrt, 1e-4f);
        }
    }
}

TEST(UtilsFixture, test_atan2_comparison)
{
    // Test cases for atan2 in different quadrants
    struct TestCase {
        float y, x;
        const char* description;
    };
    
    TestCase test_cases[] = {
        {1.0f, 1.0f, "First quadrant"},
        {1.0f, -1.0f, "Second quadrant"},
        {-1.0f, -1.0f, "Third quadrant"},
        {-1.0f, 1.0f, "Fourth quadrant"},
        {0.0f, 1.0f, "Positive x-axis"},
        {1.0f, 0.0f, "Positive y-axis"},
        {0.0f, -1.0f, "Negative x-axis"},
        {-1.0f, 0.0f, "Negative y-axis"}
    };
    
    for (const auto& test_case : test_cases) {
        IntFloat int_float_y(test_case.y);
        IntFloat int_float_x(test_case.x);
        
        float std_atan2 = std::atan2(test_case.y, test_case.x);
        
        // Placeholder for IntFloat atan2 implementation
        // Once you implement atan2 for IntFloat, replace this with actual comparison
        float int_float_atan2 = int_float_y.toFloat(); // Placeholder
        
        // This test will fail until you implement atan2 - it's here as a template
        // EXPECT_NEAR(std_atan2, int_float_atan2, 1e-4f) << "Failed for: " << test_case.description;
    }
}

// Precision and limit tests
TEST(UtilsFixture, test_precision_limits)
{
    // Test very small values
    IntFloat very_small(0.000000001); // 1e-9
    EXPECT_NEAR(0.000000001, very_small.toFloat(), 1e-10f);
    
    // Test that very small values are handled correctly
    IntFloat a(1.0f);
    IntFloat b(0.000000001);
    IntFloat sum = a + b;
    EXPECT_NEAR(1.000000001, sum.toFloat(), 1e-8f);
}

TEST(UtilsFixture, test_large_values)
{
    // Test near the limits of int32_t
    const int32_t large_int = 2000000000;
    IntFloat large_val(static_cast<float>(large_int));
    
    EXPECT_NEAR(static_cast<float>(large_int), large_val.toFloat(), 1.0f);
    
    // Test multiplication of large values
    IntFloat a(10000.0f);
    IntFloat b(100000.0f);
    IntFloat product = a * b;
    
    // Should be approximately 1e12, but might have precision issues
    EXPECT_NEAR(product.toFloat(), 1e9f, 1e-8f);
}

TEST(UtilsFixture, test_edge_cases)
{
    // Division by zero
    IntFloat a(1.0f);
    IntFloat zero(0.0f);
    IntFloat result = a / zero;
    
    // Should return zero (as per current implementation)
    EXPECT_TRUE(result.isZero());
    
    // Negative zero
    IntFloat neg_zero = -zero;
    EXPECT_TRUE(neg_zero.isZero());
    
    // Very close values
    IntFloat v1(1.00000001);
    IntFloat v2(1.00000002);
    EXPECT_TRUE(v1 < v2);
    EXPECT_FALSE(v1 == v2);
}

// Performance and accumulation error tests
TEST(UtilsFixture, test_accumulation_error)
{
    // Add a small value many times and check for error accumulation
    IntFloat sum(0.0f);
    IntFloat increment(0.000001f); // 1e-6
    const int iterations = 1000000;
    
    for (int i = 0; i < iterations; ++i) {
        sum = sum + increment;
    }
    
    EXPECT_NEAR(1.0f, sum.toFloat(), 1e-3f); // Allow some error accumulation
}

TEST(UtilsFixture, test_multiplication_precision)
{
    // Test precision of multiplication with fractions
    IntFloat a(1.0f / 3.0f);
    IntFloat b(3.0f);
    IntFloat result = a * b;
    
    // Should be very close to 1.0
    EXPECT_NEAR(1.0f, result.toFloat(), 1e-6f);
}

// Additional conversion tests
TEST(UtilsFixture, test_conversion_to_int64)
{
    IntFloat a(123.456f);
    int64_t int64_val = a.toInt64();
    
    // toInt64 returns integer * FRACT_COUNT + fract
    // So for 123.456, it should be approximately 123 * 1e9 + 456e6
    EXPECT_GT(int64_val, 123000000000LL);
    EXPECT_LT(int64_val, 124000000000LL);
}

TEST(UtilsFixture, test_get_integer_and_fraction_parts)
{
    IntFloat a(3.141592f);
    
    EXPECT_EQ(3, a.getInteger());
    // getFract returns absolute value of fractional part
    EXPECT_GT(a.getFract(), 140000000);
    EXPECT_LT(a.getFract(), 150000000);
}

// Compound assignment tests (fixed versions)
TEST(UtilsFixture, test_compound_assignment_addition)
{
    IntFloat a(5.0f);
    IntFloat b(2.5f);
    
    // Note: Currently operator+= returns a copy, not a reference
    // This might need to be fixed in the class implementation
    a += b;
    EXPECT_NEAR(7.5f, a.toFloat(), 1e-6f);
}

// Compound assignment tests (fixed versions)
TEST(UtilsFixture, test_compound_assignment_substraction)
{
    IntFloat a(5.0f);
    IntFloat b(2.5f);

    // Note: Currently operator+= returns a copy, not a reference
    // This might need to be fixed in the class implementation
    a -= b;
    EXPECT_NEAR(2.5f, a.toFloat(), 1e-6f);
}

TEST(UtilsFixture, test_compound_assignment_multiplication)
{
    IntFloat a(-674.0000f);
    IntFloat b(0.000061f);
    
    a *= b;
    
    EXPECT_NEAR(-0.041114f, a.toFloat(), 1e-6f);
}

TEST(UtilsFixture, test_compound_assignment_division)
{
    IntFloat a(5.0f);
    IntFloat b(2.5f);

    a /= b;

    EXPECT_NEAR(2.0f, a.toFloat(), 1e-6f);
}

TEST(UtilsFixture_test_absolute_value_Test, test_long_div)
{
    static const IntFloat RADS_TO_DEG = IntFloat(180, 0, 0) / IntFloat::pi();
    EXPECT_NEAR(57.295779f, RADS_TO_DEG.toFloat(), 15e-6f);
}
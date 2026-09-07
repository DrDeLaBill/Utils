#include <gtest/gtest.h>

#include <cmath>

#include "gadrc.hpp"


TEST(GadrcTest, OutputIsZeroBeforeValidConfiguration)
{
    GADRC adrc(100.0f);

    gq15_16_t out = adrc.update_1st_order_q15_16(
        gq15_16_from_int(0),
        gq15_16_from_int(10)
    );

    EXPECT_EQ(gq15_16_to_int(out), 0);
    EXPECT_FALSE(adrc.saturated());
}

TEST(GadrcTest, FirstOrderConfigurationProducesOutput)
{
    GADRC adrc(100.0f);
    adrc.setParameters(2.0f, 10.0f, 1.0f, 0.001f, 1);

    gq15_16_t out = adrc.update_1st_order_q15_16(
        gq15_16_from_int(0),
        gq15_16_from_int(10)
    );

    EXPECT_GT(gq15_16_to_float(out), 0.0f);
    EXPECT_FALSE(adrc.saturated());
}

TEST(GadrcTest, SecondOrderOutputIsClamped)
{
    GADRC adrc(25.0f);
    adrc.setParameters(20.0f, 100.0f, 1.0f, 0.001f, 2);

    gq15_16_t out = adrc.update_2nd_order_q15_16(
        gq15_16_from_int(0),
        gq15_16_from_int(100)
    );

    EXPECT_EQ(gq15_16_to_int(out), 25);
    EXPECT_TRUE(adrc.saturated());
}

TEST(GadrcTest, NegativeOutputIsClamped)
{
    GADRC adrc(25.0f);
    adrc.setParameters(20.0f, 100.0f, 1.0f, 0.001f, 1);

    gq15_16_t out = adrc.update_1st_order_q15_16(
        gq15_16_from_int(100),
        gq15_16_from_int(0)
    );

    EXPECT_EQ(gq15_16_to_int(out), -25);
    EXPECT_TRUE(adrc.saturated());
}

TEST(GadrcTest, ResetClearsStateAndSaturation)
{
    GADRC adrc(25.0f);
    adrc.setParameters(20.0f, 100.0f, 1.0f, 0.001f, 1);
    adrc.update_1st_order_q15_16(gq15_16_from_int(0), gq15_16_from_int(100));
    ASSERT_TRUE(adrc.saturated());

    adrc.reset();

    EXPECT_FALSE(adrc.saturated());
    EXPECT_EQ(gq15_16_to_int(adrc.update_1st_order_q15_16(
        gq15_16_from_int(0),
        gq15_16_from_int(10)
    )), 25);
}

TEST(GadrcTest, InvalidConfigurationLeavesPreviousParametersUnchanged)
{
    GADRC adrc(100.0f);
    adrc.setParameters(2.0f, 10.0f, 1.0f, 0.001f, 1);
    gq15_16_t expected = adrc.update_1st_order_q15_16(
        gq15_16_from_int(0),
        gq15_16_from_int(10)
    );

    adrc.setParameters(100.0f, 100.0f, 0.0f, 0.001f, 1);
    adrc.reset();
    gq15_16_t actual = adrc.update_1st_order_q15_16(
        gq15_16_from_int(0),
        gq15_16_from_int(10)
    );

    EXPECT_EQ(actual, expected);
}
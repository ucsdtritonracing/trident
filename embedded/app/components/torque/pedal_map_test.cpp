#include <gtest/gtest.h>

#include <limits>

#include "app/components/torque/pedal_map.h"

namespace Components::Torque {

// testing that map is linear
TEST(PedalMapTest, DefaultMapIsLinear) {
    const PedalMap map;

    for (std::size_t i = 0; i < PedalMap::NUM_TOTAL_POINTS; i++) {
        const float input = static_cast<float>(i) / (PedalMap::NUM_TOTAL_POINTS - 1);
        EXPECT_NEAR(input, map.getPedalOutput(input), 0.000001f);
    }
}

// changing vals doesn't affect endpoints
TEST(PedalMapTest, EndpointsStayFixed) {
    PedalMap map;
    ASSERT_TRUE(map.setPoint(0, 0.1f));
    ASSERT_TRUE(map.setPoint(PedalMap::NUM_EDITABLE_POINTS - 1, 0.1f));

    EXPECT_FLOAT_EQ(0.0f, map.getPedalOutput(0.0f));
    EXPECT_FLOAT_EQ(1.0f, map.getPedalOutput(1.0f));
    EXPECT_NEAR(1.0f, map.getPedalOutput(0.9999999f), 0.00001f);
}

// tests interpolation at exact and between points
TEST(PedalMapTest, InterpolatesBetweenPoints) {
    PedalMap map;
    const float step = 1.0f / (PedalMap::NUM_TOTAL_POINTS - 1);

    ASSERT_TRUE(map.setPoint(0, 0.4f));
    ASSERT_TRUE(map.setPoint(1, 0.8f));
    ASSERT_TRUE(map.setPoint(2, 0.9f));
    ASSERT_TRUE(map.setPoint(3, 1.0f));

    EXPECT_NEAR(0.4f, map.getPedalOutput(step), 0.00001f);
    EXPECT_NEAR(0.6f, map.getPedalOutput(1.5f * step), 0.00001f);
    EXPECT_NEAR(0.8f, map.getPedalOutput(2.0f * step), 0.00001f);
    EXPECT_NEAR(0.93f, map.getPedalOutput(3.3f * step), 0.00001f);
}

// invalid inputs should return endpoint values
TEST(PedalMapTest, HandlesInvalidInputs) {
    const PedalMap map;

    EXPECT_FLOAT_EQ(0.0f, map.getPedalOutput(-0.2f));
    EXPECT_FLOAT_EQ(0.0f, map.getPedalOutput(-std::numeric_limits<float>::infinity()));
    EXPECT_FLOAT_EQ(0.0f, map.getPedalOutput(std::numeric_limits<float>::quiet_NaN()));
    EXPECT_FLOAT_EQ(1.0f, map.getPedalOutput(1.2f));
    EXPECT_FLOAT_EQ(1.0f, map.getPedalOutput(std::numeric_limits<float>::infinity()));
}

// tests valid updates, invalid indices, invalid values
TEST(PedalMapTest, SetsEditablePoints) {
    PedalMap map;

    ASSERT_TRUE(map.setPoint(0, 0.5f));
    EXPECT_FLOAT_EQ(0.5f, map.getPoint(0));

    ASSERT_TRUE(map.setPoint(PedalMap::NUM_EDITABLE_POINTS - 1, 0.9f));
    EXPECT_FLOAT_EQ(0.9f, map.getPoint(PedalMap::NUM_EDITABLE_POINTS - 1));

    // invalid indices
    EXPECT_FALSE(map.setPoint(PedalMap::NUM_EDITABLE_POINTS, 0.5f));
    EXPECT_FALSE(map.setPoint(std::numeric_limits<std::size_t>::max(), 0.5f));

    // invalid values
    EXPECT_FALSE(map.setPoint(0, -0.1f));
    EXPECT_FALSE(map.setPoint(0, 1.1f));
    EXPECT_FALSE(map.setPoint(0, std::numeric_limits<float>::quiet_NaN()));
    EXPECT_FALSE(map.setPoint(0, std::numeric_limits<float>::infinity()));
    EXPECT_FALSE(map.setPoint(0, -std::numeric_limits<float>::infinity()));

    // rejected updates must leave the previous value the same
    EXPECT_FLOAT_EQ(0.5f, map.getPoint(0));
}

// tests that flat and increasing maps are valid
TEST(PedalMapTest, AcceptsFlatAndIncreasingMaps) {
    PedalMap map;

    EXPECT_TRUE(map.isValid());

    // flat is valid
    for (std::size_t i = 0; i < PedalMap::NUM_EDITABLE_POINTS; i++) {
        ASSERT_TRUE(map.setPoint(i, 0.5f));
    }
    EXPECT_TRUE(map.isValid());

    // increasing is valid
    for (std::size_t i = 0; i < PedalMap::NUM_EDITABLE_POINTS; i++) {
        ASSERT_TRUE(map.setPoint(i, static_cast<float>(i + 1) / (PedalMap::NUM_TOTAL_POINTS - 1)));
    }
    EXPECT_TRUE(map.isValid());
}

// decreasing points make the map invalid
TEST(PedalMapTest, RejectsDecreasingMaps) {
    PedalMap map;

    // setPoint allows decreasing values; isValid detects them.
    ASSERT_TRUE(map.setPoint(4, 0.8f));
    ASSERT_TRUE(map.setPoint(5, 0.7f));

    EXPECT_FALSE(map.isValid());
}
} // namespace Components::Torque

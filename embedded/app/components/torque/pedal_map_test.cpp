#include <gtest/gtest.h>

#include <cmath>
#include <limits>

#include "app/components/torque/pedal_map.h"

using Components::Torque::PedalMap;

//testing for linear map
TEST(PedalMapTest, DefaultMapIsLinear) {
    const PedalMap map;

    for (std::size_t i = 0; i < PedalMap::NUM_TOTAL_POINTS; i++) {
        const float input = static_cast<float>(i) / (PedalMap::NUM_TOTAL_POINTS - 1);
        EXPECT_NEAR(input, map.getPedalOutput(input), 0.000001f);
    }
}


#pragma once

#include <cstddef>

namespace Components::Torque {
class PedalMap {
public:
    static constexpr std::size_t NUM_EDITABLE_POINTS = 16;
    static constexpr std::size_t NUM_TOTAL_POINTS = NUM_EDITABLE_POINTS + 2;

    PedalMap();

    float getPedalOutput(float pedalInput) const;
    float getPoint(std::size_t index) const;
    bool setPoint(std::size_t index, float value);
    bool isValid() const;

private:
    float points[NUM_TOTAL_POINTS];
};
} // namespace Components::Torque
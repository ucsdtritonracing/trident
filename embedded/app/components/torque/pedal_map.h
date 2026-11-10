#pragma once

#include <cstddef>

namespace Components::Torque {

/**
 * @brief Initializes a default linear pedal map.
 */
class PedalMap {
public:
    static constexpr std::size_t NUM_EDITABLE_POINTS = 16;
    static constexpr std::size_t NUM_TOTAL_POINTS = NUM_EDITABLE_POINTS + 2;

    PedalMap();

    /**
     * @brief Converts pedal input into torque output.
     * @param pedalInput Normalized pedal input between 0.0 and 1.0.
     * @return Interpolated torque output between 0.0 and 1.0.
     *         Returns 0.0 for negative or NaN inputs and 1.0 for inputs >= 1.0.
     */
    float getPedalOutput(float pedalInput) const;
    /**
     * @brief Gets the value of an editable mapping point.
     * @param index Zero-based index of the editable point.
     * @return Point value, or 0.0 if the index is invalid.
     */
    float getPoint(std::size_t index) const;
    /**
     * @brief Updates the value of an editable mapping point.
     * @param index Zero-based index of the editable point.
     * @param value New point value between 0.0 and 1.0.
     * @return true if updated successfully, false if the index or value is invalid.
     */
    bool setPoint(std::size_t index, float value);
    /**
     * @brief Checks whether the mapping points are in nondecreasing order.
     * @return true if every point is greater than or equal to the previous point.
     */
    bool isValid() const;

private:
    float points[NUM_TOTAL_POINTS];
};
} // namespace Components::Torque
#include "pedal_map.h"
#include <cmath>

namespace Components::Torque {
/**
 * @brief Initializes a default linear pedal map.
 */
PedalMap::PedalMap() {
    for (std::size_t i = 0; i < NUM_TOTAL_POINTS; i++) {
        points[i] = static_cast<float>(i) / (NUM_TOTAL_POINTS - 1);
    }
}
/**
 * @brief Converts pedal input into torque output.
 * @param pedalInput Normalized pedal input between 0.0 and 1.0.
 * @return Interpolated torque output between 0.0 and 1.0.
 *         Returns 0.0 for negative or NaN inputs and 1.0 for inputs >= 1.0.
 */
float PedalMap::getPedalOutput(float pedalInput) const {
    if (pedalInput < 0.0f || std::isnan(pedalInput)) {
        return 0.0f;
    }
    if (pedalInput >= 1.0f) {
        return 1.0f;
    }

    float position = pedalInput * (NUM_TOTAL_POINTS - 1);
    std::size_t lowerIndex = static_cast<std::size_t>(position);

    if (lowerIndex >= NUM_TOTAL_POINTS - 1) {
        return points[NUM_TOTAL_POINTS - 1];
    }

    std::size_t upperIndex = lowerIndex + 1;

    float fraction = position - lowerIndex;
    return points[lowerIndex] + fraction * (points[upperIndex] - points[lowerIndex]);
}

/**
 * @brief Gets the value of an editable mapping point.
 * @param index Zero-based index of the editable point.
 * @return Point value, or 0.0 if the index is invalid.
 */

float PedalMap::getPoint(std::size_t index) const {
    if (index >= NUM_EDITABLE_POINTS) {
        return 0.0f;
    } else {
        return points[index + 1];
    }
}

/**
 * @brief Updates the value of an editable mapping point.
 * @param index Zero-based index of the editable point.
 * @param value New point value between 0.0 and 1.0.
 * @return true if updated successfully, false if the index or value is invalid.
 */

bool PedalMap::setPoint(std::size_t index, float value) {
    if (index >= NUM_EDITABLE_POINTS || std::isnan(value) || value < 0.0f || value > 1.0f) {
        return false;
    }

    points[index + 1] = value;
    return true;
}

/**
 * @brief Checks whether the mapping points are in nondecreasing order.
 * @return true if every point is greater than or equal to the previous point.
 */

bool PedalMap::isValid() const {
    for (std::size_t i = 0; i < NUM_TOTAL_POINTS - 1; i++) {
        if (points[i + 1] < points[i]) {
            return false;
        }
    }

    return true;
}

} // namespace Components::Torque

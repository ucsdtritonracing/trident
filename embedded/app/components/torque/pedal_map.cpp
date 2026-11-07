#include "pedal_map.h"
#include <cmath>

namespace Components::Torque {

PedalMap::PedalMap() { // default constructor makes a linear map
    for (std::size_t i = 0; i < NUM_TOTAL_POINTS; i++) {
        points[i] = static_cast<float>(i) / (NUM_TOTAL_POINTS - 1);
    }
}

float PedalMap::getPedalOutput(float pedalInput) const { // pedal input to torque output
    if (pedalInput < 0.0f || std::isnan(pedalInput)) {
        return 0.0f;
    }
    if (pedalInput >= 1.0f) {
        return 1.0f;
    }

    float position = pedalInput * (NUM_TOTAL_POINTS - 1);
    std::size_t lowerIndex = static_cast<std::size_t>(position);

    if (lowerIndex >= NUM_TOTAL_POINTS - 1) {
        return points[NUM_TOTAL_POINTS - 1]; // just in case?
    }

    std::size_t upperIndex = lowerIndex + 1;

    // calculate the fraction between the two points
    float fraction = position - lowerIndex;
    return points[lowerIndex] + fraction * (points[upperIndex] - points[lowerIndex]);
}

float PedalMap::getPoint(std::size_t index) const {
    if (index >= NUM_EDITABLE_POINTS) { // size_t can't be negative so don't have to check 0
        return 0.0f;
    } else {
        return points[index + 1]; // array has a fixed endpoints, shifting every pos by one
    }
}
bool PedalMap::setPoint(std::size_t index,
                        float value) { // cannot change the value of fixed endpoints
    if (index >= NUM_EDITABLE_POINTS || std::isnan(value) || value < 0.0f || value > 1.0f) {
        return false;
    }

    points[index + 1] = value;
    return true;
}
bool PedalMap::isValid() const { // only after updates r done
    for (std::size_t i = 0; i < NUM_TOTAL_POINTS - 1; i++) {
        if (points[i + 1] < points[i]) {
            return false;
        }
    }

    return true;
}

} // namespace Components::Torque

// it should never access a point outside the array for any flaot input (?)

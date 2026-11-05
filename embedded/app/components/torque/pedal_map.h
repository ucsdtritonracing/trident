#pragma once

#include <cstddef.h> 

namepace Components:: Torque {
    class PedalMap{
        public:
        static constexpr size_t NUM_EDITABLE_POINTS = 16;
        static constexpr size_t NUM_TOTAL_POINTS = NUM_EDITABLE_POINTS + 2;

        PedalMap();

        float getPedalOutput(float pedalInput) const; 
        float getPoint(size_t index) const;
        bool setPoint(size_t index, float value);
        bool isValid() const;

    }
}
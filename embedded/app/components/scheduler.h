#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "app/components/component.h"
#include "core/periodic.h"
#include "core/time.h"

namespace App::Components {

/**
 * @brief Represents a component and its execution rate in the scheduler's table.
 */
struct Scheduled {
    Component& component;
    Core::Periodic rate;
};

/**
 * @brief Runs a fixed table of components at their configured rates.
 *
 * - Components run in the order they are defined within the table.
 * - If a component falls behind by more than one period, the missed runs are skipped.
 *
 * @tparam N Number of components in the table. Deduced automatically from the `std::array` passed
 *           to the constructor, so it never needs to be written out.
 */
template <std::size_t N> class Scheduler {
public:
    constexpr explicit Scheduler(std::array<Scheduled, N> entries) : entries(entries) {}

    /**
     * @brief Executes all components that are due at the given timestamp.
     * @param now The current timestamp.
     */
    void Run(Core::Timestamp now) {
        for (Scheduled& entry : entries) {
            if (entry.rate.Due(now)) {
                entry.component.Step(now);
            }
        }
    }

    /**
     * @brief Returns the total number of missed runs across all components.
     */
    [[nodiscard]] uint32_t MissedCount() const {
        uint32_t total = 0;
        for (const Scheduled& entry : entries) {
            total += entry.rate.MissedCount();
        }
        return total;
    }

private:
    std::array<Scheduled, N> entries;
};

} // namespace App::Components

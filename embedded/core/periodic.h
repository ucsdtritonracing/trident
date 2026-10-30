#pragma once

#include <cstdint>

#include "core/time.h"

namespace Core {

/**
 * A periodic timer that triggers at regular intervals.
 *
 * @note Example usage:
 * @code
 * static Core::Periodic rate{10ms};
 * void Step(Core::Timestamp now) {
 *      if (!rate.Due(now)) {
 *          return;
 *      }
 *      ...
 * }
 * @endcode
 */
class Periodic {
public:
    /**
     * @param period Time between runs. Fixed for the lifetime of the Periodic.
     * @param offset Delay of the first due time after the first call. Used to spread components
     *               with the same period across different ticks.
     */
    constexpr explicit Periodic(Duration period, Duration offset = Duration::zero())
        : period(period), offset(offset) {}

    /**
     * Check whether the caller is due to run.
     *
     * @param now The current time.
     * @return True if the caller should run this tick.
     *
     * @note The [[nodiscard]] is to prevent accidentally ignoring the return value, because the
     * next_due is updated on every call.
     */
    [[nodiscard]] bool Due(Timestamp now) {
        // Always run if the period is zero/negative.
        if (period <= Duration::zero()) {
            return true;
        }

        if (!started) {
            next_due = now + offset;
            started = true;
        }

        if (now < next_due) {
            return false;
        }

        next_due += period;
        if (now >= next_due) {
            // Fell behind by at least a whole period.
            const auto skipped = ((now - next_due) / period) + 1;
            next_due += skipped * period;
            missed_count += static_cast<uint32_t>(skipped);
        }
        return true;
    }

    /**
     * @return The number of runs skipped because whole periods were missed.
     */
    [[nodiscard]] uint32_t MissedCount() const { return missed_count; }

private:
    Duration period;
    Duration offset;
    Timestamp next_due;
    uint32_t missed_count = 0;
    bool started = false;
};

} // namespace Core

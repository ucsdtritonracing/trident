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
    /// Period and offset for a Periodic. Usage: `Periodic{{.period = 10ms, .offset = 2ms}}`
    struct Config {
        /// Time between runs. Fixed for the lifetime of the Periodic.
        Duration period;
        /// Delay of the first due time after the first call. Used to spread components with the
        /// same period across different ticks.
        Duration offset = Duration::zero();
    };
    /**
     * @param period Time between runs. Fixed for the lifetime of the Periodic.
     */
    constexpr explicit Periodic(Duration period) : period(period) {}
    /**
     * @param config Period and offset. See Config.
     */
    constexpr explicit Periodic(Config config) : period(config.period), offset(config.offset) {}
    /**
     * Check whether the caller is due to run.
     *
     * @param now The current time.
     * @return True if the caller should run this tick.
     *
     * @note The [[nodiscard]] is to prevent the caller from ignoring the return value, because
     * next_due is updated on every call. This guarantees the periodic doesn't blindly advance.
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
    Duration offset = Duration::zero();
    Timestamp next_due;
    uint32_t missed_count = 0;
    bool started = false;
};

} // namespace Core
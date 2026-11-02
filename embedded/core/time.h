#pragma once

#include <chrono>
#include <cstdint>

/**
 * @note Make the chrono suffixes (e.g. 10ms, 500us, etc.) available everywhere time.h is included.
 * Normally, a global `using namespace` is bad practice, but the chrono suffixes are reserved for
 * the standard library so they won't collide with any of our names.
 */
using namespace std::chrono_literals;

namespace Core {

struct SystemClock {
    using rep = int64_t;
    using period = std::micro;
    using duration = std::chrono::duration<int64_t, period>;
    using time_point = std::chrono::time_point<SystemClock>;
};

using Duration = SystemClock::duration;
using Timestamp = SystemClock::time_point;

/**
 * Create a timestamp from a microsecond count.
 * @param us The microsecond count.
 * @return The corresponding timestamp.
 */
constexpr Timestamp FromMicros(int64_t us) { return Timestamp{Duration{us}}; }

/**
 * Convert a timestamp to microseconds as an integer.
 * @param t The timestamp to convert.
 * @return The timestamp in microseconds as an integer.
 */
constexpr int64_t ToMicros(Timestamp t) { return t.time_since_epoch().count(); }

/**
 * Convert a duration to microseconds as an integer.
 * @param d The duration to convert.
 * @return The duration in microseconds as an integer.
 */
constexpr int64_t ToMicros(Duration d) { return d.count(); }

/**
 * Convert a duration to seconds as a float.
 * @param d The duration to convert.
 * @return The duration in seconds as a float.
 */
constexpr float ToSeconds(Duration d) { return std::chrono::duration<float>(d).count(); }

} // namespace Core

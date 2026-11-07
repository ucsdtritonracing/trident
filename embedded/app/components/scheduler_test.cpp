#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <vector>

#include "app/components/component.h"
#include "app/components/scheduler.h"
#include "core/periodic.h"
#include "core/time.h"

using App::Components::Component;
using App::Components::Scheduled;
using App::Components::Scheduler;
using Core::Duration;
using Core::Periodic;
using Core::Timestamp;

namespace {

/**
 * @brief For every Step() call, this component records the timestamp and its id in a shared log to
 * check run order.
 */
class RecorderComponent : public Component {
public:
    explicit RecorderComponent(int id = 0, std::vector<int>* order = nullptr)
        : id(id), order(order) {}

    void Step(Timestamp now) override {
        timestamps.push_back(now);
        if (order != nullptr) {
            order->push_back(id);
        }
    }

    std::vector<Timestamp> timestamps;

private:
    int id;
    std::vector<int>* order;
};

/**
 * @brief Helper function to call Run() every `tick` from time zero up to (but not including) `end`.
 * @tparam N The number of components in the scheduler.
 * @param scheduler The scheduler to run.
 * @param end The time at which to stop running.
 * @param tick The interval at which to call Run().
 */
template <std::size_t N> void RunFor(Scheduler<N>& scheduler, Duration end, Duration tick = 1ms) {
    for (Duration t = Duration::zero(); t < end; t += tick) {
        scheduler.Run(Timestamp{t});
    }
}

} // namespace

TEST(Scheduler, RunsComponentsInOrder) {
    std::vector<int> order;
    RecorderComponent first{1, &order};
    RecorderComponent second{2, &order};
    RecorderComponent third{3, &order};

    // Purposely not sorted by id. The order of components in the table is what matters.
    Scheduler scheduler{std::array{
        Scheduled{.component = third, .rate = Periodic{10ms}},
        Scheduled{.component = first, .rate = Periodic{10ms}},
        Scheduled{.component = second, .rate = Periodic{10ms}},
    }};
    scheduler.Run(Timestamp{0ms});

    EXPECT_EQ(order, (std::vector<int>{3, 1, 2}));
}

TEST(Scheduler, RunsEachComponentAtItsOwnPeriod) {
    RecorderComponent fast;
    RecorderComponent slow;
    Scheduler scheduler{std::array{
        Scheduled{.component = fast, .rate = Periodic{10ms}},
        Scheduled{.component = slow, .rate = Periodic{20ms}},
    }};

    RunFor(scheduler, 100ms);

    EXPECT_EQ(fast.timestamps.size(), 10); // Should be 0, 10, ..., 90ms
    EXPECT_EQ(slow.timestamps.size(), 5);  // Should be 0, 20, ..., 80ms
}

TEST(Scheduler, PassesCurrentTimeToStep) {
    RecorderComponent recorder;
    Scheduler scheduler{std::array{Scheduled{.component = recorder, .rate = Periodic{10ms}}}};

    RunFor(scheduler, 30ms);

    EXPECT_EQ(recorder.timestamps,
              (std::vector<Timestamp>{Timestamp{0ms}, Timestamp{10ms}, Timestamp{20ms}}));
}

TEST(Scheduler, OffsetDelaysFirstRun) {
    RecorderComponent plain;
    RecorderComponent offset;
    Scheduler scheduler{std::array{
        Scheduled{.component = plain, .rate = Periodic{10ms}},
        Scheduled{.component = offset, .rate = Periodic{{.period = 10ms, .offset = 2ms}}},
    }};

    RunFor(scheduler, 30ms);

    EXPECT_EQ(plain.timestamps,
              (std::vector<Timestamp>{Timestamp{0ms}, Timestamp{10ms}, Timestamp{20ms}}));
    EXPECT_EQ(offset.timestamps,
              (std::vector<Timestamp>{Timestamp{2ms}, Timestamp{12ms}, Timestamp{22ms}}));
}

TEST(Scheduler, ComponentNotRunBetweenPeriods) {
    RecorderComponent recorder;
    Scheduler scheduler{std::array{Scheduled{.component = recorder, .rate = Periodic{10ms}}}};

    scheduler.Run(Timestamp{0ms});
    scheduler.Run(Timestamp{5ms});
    scheduler.Run(Timestamp{9'999us});

    EXPECT_EQ(recorder.timestamps.size(), 1);
}

TEST(Scheduler, MissedCountSumsAcrossComponents) {
    RecorderComponent a;
    RecorderComponent b;
    Scheduler scheduler{std::array{
        Scheduled{.component = a, .rate = Periodic{10ms}},
        Scheduled{.component = b, .rate = Periodic{10ms}},
    }};

    scheduler.Run(Timestamp{0ms});  // Both run, next due at 10 ms
    scheduler.Run(Timestamp{35ms}); // Both missed the 10 ms and 20 ms runs

    EXPECT_EQ(scheduler.MissedCount(), 4);
    // Missed runs are skipped, not run back to back.
    EXPECT_EQ(a.timestamps.size(), 2);
    EXPECT_EQ(b.timestamps.size(), 2);
}

TEST(Scheduler, MissedCountZeroWhenOnTime) {
    RecorderComponent recorder;
    Scheduler scheduler{std::array{Scheduled{.component = recorder, .rate = Periodic{10ms}}}};

    RunFor(scheduler, 100ms);

    EXPECT_EQ(scheduler.MissedCount(), 0);
}

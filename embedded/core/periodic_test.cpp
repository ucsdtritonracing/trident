#include <gtest/gtest.h>

#include "core/periodic.h"
#include "core/time.h"

// The tests are split into tests using Periodic(Duration) and tests using Periodic(Config) so
// basically I'm repeating tests for with offset vs without.

// Periodic(Duration) test for a zero period.
TEST(PeriodicTest, ZeroPeriodAlwaysDue) {
    Core::Periodic periodic{0ms};

    EXPECT_TRUE(periodic.Due(Core::Timestamp{0ms}));
    EXPECT_TRUE(periodic.Due(Core::Timestamp{1ms}));
    EXPECT_TRUE(periodic.Due(Core::Timestamp{10ms}));
}

// Periodic(Duration) test for a negative period.
TEST(PeriodicTest, NegativePeriodAlwaysDue) {
    Core::Periodic periodic{-5ms};

    EXPECT_TRUE(periodic.Due(Core::Timestamp{0ms}));
    EXPECT_TRUE(periodic.Due(Core::Timestamp{1ms}));
    EXPECT_TRUE(periodic.Due(Core::Timestamp{10ms}));
}

// Periodic(Duration) test for a first call with no offset.
TEST(PeriodicTest, FirstCallNoOffsetIsDue) {
    Core::Periodic periodic{5ms};

    EXPECT_TRUE(periodic.Due(Core::Timestamp{5ms}));
}

// Periodic(Config) test for a first call with an offset.
TEST(PeriodicTest, FirstCallWithOffsetNotDue) {
    Core::Periodic periodic{{.period = 5ms, .offset = 2ms}};

    EXPECT_FALSE(periodic.Due(Core::Timestamp{5ms}));
    EXPECT_TRUE(periodic.Due(Core::Timestamp{7ms}));
}

// Periodic(Duration) test for a call before the deadline.
TEST(PeriodicTest, BeforeDeadlineIsNotDue) {
    Core::Periodic periodic{10ms};

    EXPECT_TRUE(periodic.Due(Core::Timestamp{1ms}));
    EXPECT_FALSE(periodic.Due(Core::Timestamp{5ms}));
    EXPECT_FALSE(periodic.Due(Core::Timestamp{9ms}));
}

// Periodic(Duration) test for a call right at the deadline.
TEST(PeriodicTest, RightAtDeadlineIsDue) {
    Core::Periodic periodic{8ms};

    EXPECT_TRUE(periodic.Due(Core::Timestamp{8ms}));
    EXPECT_TRUE(periodic.Due(Core::Timestamp{16ms}));
}

// Periodic(Duration) test for a call slightly after the deadline without a miss.
TEST(PeriodicTest, SlightlyAfterDeadlineIsDueWithoutMiss) {
    Core::Periodic periodic{10ms};

    EXPECT_TRUE(periodic.Due(Core::Timestamp{0ms}));
    EXPECT_TRUE(periodic.Due(Core::Timestamp{11ms}));
    EXPECT_TRUE(periodic.Due(Core::Timestamp{21ms}));
    EXPECT_EQ(0, periodic.MissedCount());
}

// Periodic(Duration) test for a call one period after the deadline.
TEST(PeriodicTest, OnePeriodAfterDeadlineRecordsMiss) {
    Core::Periodic periodic{10ms};

    EXPECT_TRUE(periodic.Due(Core::Timestamp{0ms}));
    EXPECT_TRUE(periodic.Due(Core::Timestamp{20ms}));
    EXPECT_EQ(1, periodic.MissedCount());

    EXPECT_TRUE(periodic.Due(Core::Timestamp{41ms}));
    EXPECT_EQ(2, periodic.MissedCount());
}

// Periodic(Duration) test for a call multiple periods after the deadline.
TEST(PeriodicTest, MultiplePeriodsAfterDeadlineRecordsMisses) {
    Core::Periodic periodic{10ms};

    EXPECT_TRUE(periodic.Due(Core::Timestamp{0ms}));
    EXPECT_TRUE(periodic.Due(Core::Timestamp{40ms}));
    EXPECT_EQ(3, periodic.MissedCount());
}

// Periodic(Duration) test for repeated calls at the same timestamp.
TEST(PeriodicTest, RepeatedCallAtSameTimestampIsNotDue) {
    Core::Periodic periodic{10ms};

    EXPECT_TRUE(periodic.Due(Core::Timestamp{0ms}));
    EXPECT_TRUE(periodic.Due(Core::Timestamp{10ms}));
    EXPECT_FALSE(periodic.Due(Core::Timestamp{10ms}));
}

// Periodic(Config) test for a call before the deadline with an offset.
TEST(PeriodicTest, BeforeDeadlineWithOffsetIsNotDue) {
    Core::Periodic periodic{{.period = 10ms, .offset = 2ms}};

    EXPECT_FALSE(periodic.Due(Core::Timestamp{0ms}));
    EXPECT_FALSE(periodic.Due(Core::Timestamp{1ms}));
}

// Periodic(Config) test for a call right at the deadline with an offset.
TEST(PeriodicTest, RightAtDeadlineWithOffsetIsDue) {
    Core::Periodic periodic{{.period = 10ms, .offset = 2ms}};

    EXPECT_FALSE(periodic.Due(Core::Timestamp{0ms}));
    EXPECT_TRUE(periodic.Due(Core::Timestamp{2ms}));
}

// Periodic(Config) test for a call slightly after the deadline without a miss.
TEST(PeriodicTest, SlightlyAfterDeadlineWithOffsetIsDueWithoutMiss) {
    Core::Periodic periodic{{.period = 10ms, .offset = 2ms}};

    EXPECT_FALSE(periodic.Due(Core::Timestamp{0ms}));
    EXPECT_TRUE(periodic.Due(Core::Timestamp{3ms}));
    EXPECT_EQ(0, periodic.MissedCount());
}

// Periodic(Config) test for a call one period after the deadline.
TEST(PeriodicTest, OnePeriodAfterDeadlineWithOffsetRecordsMiss) {
    Core::Periodic periodic{{.period = 10ms, .offset = 2ms}};

    EXPECT_FALSE(periodic.Due(Core::Timestamp{0ms}));
    EXPECT_TRUE(periodic.Due(Core::Timestamp{12ms}));
    EXPECT_EQ(1, periodic.MissedCount());
}

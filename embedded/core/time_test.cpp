#include <gtest/gtest.h>

#include "core/time.h"


// FromMicros(int64_t) tests for positive and zero values.
TEST(TimeTest, FromMicrosPositive) {
    const Core::Timestamp timestamp = Core::FromMicros(900);
    EXPECT_EQ(Core::Duration{900}, timestamp.time_since_epoch());
}

TEST(TimeTest, FromMicrosZero) {
    const Core::Timestamp timestamp = Core::FromMicros(0);
    EXPECT_EQ(Core::Duration::zero(), timestamp.time_since_epoch());
}


// ToMicros(Timestamp) tests for positive and zero values.
TEST(TimeTest, TimestampToMicrosPositive) {
    const Core::Timestamp timestamp{Core::Duration{900}};
    EXPECT_EQ(900, Core::ToMicros(timestamp));
}

TEST(TimeTest, TimestampToMicrosZero) {
    const Core::Timestamp timestamp{Core::Duration{0}};
    EXPECT_EQ(0, Core::ToMicros(timestamp));
}


// ToMicros(Duration) tests for positive and zero values.
TEST(TimeTest, DurationToMicrosPositive) {
    const Core::Duration duration = 2ms;
    EXPECT_EQ(2000, Core::ToMicros(duration));
}

TEST(TimeTest, DurationToMicrosZero) {
    const Core::Duration duration = Core::Duration::zero();
    EXPECT_EQ(0, Core::ToMicros(duration));
}


// ToSeconds(Duration) tests for positive and zero values.
TEST(TimeTest, ToSecondsPositive) {
    const Core::Duration duration = 500ms;
    EXPECT_FLOAT_EQ(0.5F, Core::ToSeconds(duration));
}

TEST(TimeTest, ToSecondsZero) {
    const Core::Duration duration = 0ms;
    EXPECT_FLOAT_EQ(0.0F, Core::ToSeconds(duration));
}
#include "utl/cast/Narrow.hpp"

#include <cstdint>
#include <limits>

#include <gtest/gtest.h>

namespace mage::utl::test
{

class NarrowTest : public ::testing::Test
{
protected:
    static constexpr int64_t SafeValue = 1000;
    static constexpr int64_t OverflowValue = 5'000'000'000LL;
    static constexpr int64_t UnderflowValue = -5'000'000'000LL;
};

TEST_F(NarrowTest, SafeConversion)
{
    const int64_t myValue = SafeValue;
    const int32_t myResult = narrow<int32_t>(myValue);
    EXPECT_EQ(myResult, 1000);
}

TEST_F(NarrowTest, ThrowsOnOverflow)
{
    const int64_t myBigValue = OverflowValue; // Exceeds int32_t max
    EXPECT_THROW(narrow<int32_t>(myBigValue), gsl::narrowing_error);
}

TEST_F(NarrowTest, ThrowsOnUnderflow)
{
    const int64_t mySmallValue = UnderflowValue; // Below int32_t min
    EXPECT_THROW(narrow<int32_t>(mySmallValue), gsl::narrowing_error);
}

TEST_F(NarrowTest, ThrowsOnSignednessChange)
{
    const int32_t myNegative = -1;
    EXPECT_THROW(narrow<uint32_t>(myNegative), gsl::narrowing_error);
}

TEST_F(NarrowTest, MaxValueConversion)
{
    const int32_t myMax = std::numeric_limits<int32_t>::max();
    const int32_t myResult = narrow<int32_t>(static_cast<int64_t>(myMax));
    EXPECT_EQ(myResult, myMax);
}

class NarrowCastTest : public ::testing::Test
{
protected:
    // Helper values that can be used across tests
    static constexpr int64_t SafeValue = 1000;
    static constexpr int64_t OverflowValue = 5'000'000'000LL;
};

TEST_F(NarrowCastTest, NoThrowOnOverflow)
{
    const int64_t myBigValue = OverflowValue;
    // narrow_cast doesn't throw, just truncates
    EXPECT_NO_THROW(narrow_cast<int32_t>(myBigValue));
}

TEST_F(NarrowCastTest, ExplicitConversion)
{
    const int64_t myValue = SafeValue;
    const int32_t myResult = narrow_cast<int32_t>(myValue);
    EXPECT_EQ(myResult, 1000);
}

TEST_F(NarrowCastTest, Constexpr)
{
    constexpr int64_t myValue = 42;
    constexpr int32_t myResult = narrow_cast<int32_t>(myValue);
    EXPECT_EQ(myResult, 42);
}

} // namespace mage::utl::test

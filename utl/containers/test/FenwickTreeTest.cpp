#include "utl/containers/FenwickTree.hpp"

#include <gtest/gtest.h>

namespace mage::utl::test
{
class FenwickTreeTest : public ::testing::Test
{
public:
    void setUpForInt64()
    {
        theVictim = std::make_unique<FenwickTree<std::int64_t, MaxSize>>(
            [](const auto& aFirst, const auto& aSecond)
            {
                return aFirst + aSecond;
            });
    }

    static constexpr std::int64_t MaxSize = 1e6;
    std::unique_ptr<FenwickTree<std::int64_t, MaxSize>> theVictim;
};

TEST_F(FenwickTreeTest, CheckInsertOneElement)
{
    setUpForInt64();
    theVictim->insertDiffAt(0, 100);
    EXPECT_EQ(theVictim->getAccumulation(0), 100);
}

TEST_F(FenwickTreeTest, CheckInsertTwoElements)
{
    setUpForInt64();
    theVictim->insertDiffAt(0, 100);
    theVictim->insertDiffAt(1, 100);
    EXPECT_EQ(theVictim->getAccumulation(0), 100);
    EXPECT_EQ(theVictim->getAccumulation(1), 200);
}

TEST_F(FenwickTreeTest, TreeSupportForMultiplication)
{
    auto myVictimWithMulSupport = std::make_unique<FenwickTree<std::int64_t, MaxSize, FillWithOnes::Enabled>>(
        [](const auto& aFirst, const auto& aSecond)
        {
            return aFirst * aSecond;
        });
    myVictimWithMulSupport->insertDiffAt(0, 1);
    myVictimWithMulSupport->insertDiffAt(1, 2);
    myVictimWithMulSupport->insertDiffAt(2, 3);
    EXPECT_EQ(myVictimWithMulSupport->getAccumulation(0), 1);
    EXPECT_EQ(myVictimWithMulSupport->getAccumulation(1), 2);
    EXPECT_EQ(myVictimWithMulSupport->getAccumulation(2), 6);
}

TEST_F(FenwickTreeTest, TreeSupportsUpdate)
{
    setUpForInt64();
    theVictim->insertDiffAt(0, 100);
    theVictim->insertDiffAt(1, 100);
    theVictim->insertDiffAt(2, 100);
    EXPECT_EQ(theVictim->getAccumulation(0), 100);
    EXPECT_EQ(theVictim->getAccumulation(1), 200);
    EXPECT_EQ(theVictim->getAccumulation(2), 300);
    theVictim->insertDiffAt(0, 100);
    EXPECT_EQ(theVictim->getAccumulation(0), 200);
    EXPECT_EQ(theVictim->getAccumulation(1), 300);
    EXPECT_EQ(theVictim->getAccumulation(2), 400);
}
} // namespace mage::utl::test

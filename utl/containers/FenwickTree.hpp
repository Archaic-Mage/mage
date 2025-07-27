#pragma once

#include <array>
#include <cstdint>
#include <functional>

namespace mage::utl
{

enum class FillWithOnes
{
    Enabled,
    Disabled,
};

template <typename TypeT, std::int64_t SizeT, FillWithOnes FillWithOnesV = FillWithOnes::Disabled>
class FenwickTree
{
    // This operation must be binary associative.
    using Operation = std::function<TypeT(const TypeT&, const TypeT&)>;

public:
    explicit FenwickTree(Operation anOperation);

    void insertDiffAt(std::int64_t anIndex, TypeT aDiff);

    TypeT getAccumulation(std::int64_t anIndex) const;

private:
    Operation theOperation;
    std::array<TypeT, SizeT> theData{};
};

template <typename TypeT, std::int64_t SizeT, FillWithOnes FillWithOnesV>
FenwickTree<TypeT, SizeT, FillWithOnesV>::FenwickTree(Operation anOperation) : theOperation(std::move(anOperation))
{
    for (std::int64_t anIndex = 0; anIndex < SizeT; ++anIndex)
    {
        theData[anIndex] = FillWithOnesV == FillWithOnes::Enabled ? 1 : 0;
    }
}

template <typename TypeT, std::int64_t SizeT, FillWithOnes FillWithOnesV>
void FenwickTree<TypeT, SizeT, FillWithOnesV>::insertDiffAt(std::int64_t anIndex, TypeT aDiff)
{
    for (; anIndex < SizeT; anIndex = anIndex | (anIndex + 1))
    {
        theData[anIndex] = theOperation(theData[anIndex], aDiff);
    }
}

template <typename TypeT, std::int64_t SizeT, FillWithOnes FillWithOnesV>
TypeT FenwickTree<TypeT, SizeT, FillWithOnesV>::getAccumulation(std::int64_t anIndex) const
{
    TypeT myResult = FillWithOnesV == FillWithOnes::Enabled ? 1 : 0;
    for (; anIndex >= 0; anIndex = (anIndex & (anIndex + 1)) - 1)
    {
        myResult = theOperation(myResult, theData[anIndex]);
    }
    return myResult;
}
} // namespace mage::utl

#pragma once

#include <array>
#include <functional>

namespace mage::utl
{

template <typename TypeT, std::int64_t SizeT>
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

template <typename TypeT, std::int64_t SizeT>
FenwickTree<TypeT, SizeT>::FenwickTree(Operation anOperation) : theOperation(std::move(anOperation))
{
}

template <typename TypeT, std::int64_t SizeT>
void FenwickTree<TypeT, SizeT>::insertDiffAt(std::int64_t anIndex, TypeT aDiff)
{
    for (; anIndex < SizeT; anIndex = anIndex | (anIndex + 1))
    {
        theData[anIndex] = theOperation(theData[anIndex], aDiff);
    }
}

template <typename TypeT, std::int64_t SizeT>
TypeT FenwickTree<TypeT, SizeT>::getAccumulation(std::int64_t anIndex) const
{
    TypeT myResult{};
    for (; anIndex >= 0; anIndex = (anIndex & (anIndex + 1)) - 1)
    {
        myResult = theOperation(myResult, theData[anIndex]);
    }
    return myResult;
}
} // namespace mage::utl

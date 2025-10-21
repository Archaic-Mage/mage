#pragma once

#include <gsl/narrow>
#include <gsl/util>

namespace mage::utl
{

/// @brief Safely narrow a numeric value, throwing if data would be lost
/// @throws gsl::narrowing_error if the conversion would lose data
///
/// Example:
///   int64_t myBig = 5'000'000'000LL;
///   int32_t mySmall = mage::utl::narrow<int32_t>(myBig);  // throws!
template <typename To, typename From>
constexpr To narrow(From aValue)
{
    return gsl::narrow<To>(aValue);
}

// NOLINTBEGIN(*)
/// @brief Explicitly narrow a numeric value without runtime checks
///
/// Use this when you know the conversion is safe but want to be explicit.
/// Prefer narrow<T>() when safety is important.
///
/// Example:
///   int64_t myValue = 1000;
///   int32_t mySmall = mage::utl::narrow_cast<int32_t>(myValue);  // ok
template <typename To, typename From>
constexpr To narrow_cast(From aValue) noexcept
{
    return gsl::narrow_cast<To>(aValue);
}
// NOLINTEND(*)

} // namespace mage::utl

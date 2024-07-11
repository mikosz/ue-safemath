#pragma once

namespace SafeMath
{

template <class T>
constexpr T SmallNumber;

template <>
constexpr auto SmallNumber<float> = UE_SMALL_NUMBER;

template <>
constexpr auto SmallNumber<double> = UE_DOUBLE_SMALL_NUMBER;

template <class T>
constexpr std::enable_if_t<std::is_floating_point_v<T>, T> Abs(T Value);

template <class T>
constexpr std::enable_if_t<std::is_floating_point_v<T>, bool> NearlyEqual(
	T Lhs, T Rhs, T ErrorTolerance = SmallNumber<T>);

template <class T>
constexpr bool NearlyEqual(
	const UE::Math::TVector<T>& Lhs, const UE::Math::TVector<T>& Rhs, T ErrorTolerance = SmallNumber<T>);

}  // namespace SafeMath

#include "Utils.inc"

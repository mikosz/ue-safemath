#pragma once

#include "Value.h"

namespace SafeMath
{

template <class InUnitType, class InValueType>
constexpr TValue<InUnitType, InValueType>::TValue(UnitType, ValueType InValue) : Value{InValue}
{
}

template <class T>
constexpr std::enable_if_t<std::is_floating_point_v<T>, T> Abs(T Value)
{
	return Value < T{0} ? -Value : Value;
}

template <class T>
constexpr std::enable_if_t<std::is_floating_point_v<T>, bool> NearlyEqual(T Lhs, T Rhs, T ErrorTolerance)
{
	return Abs(Lhs - Rhs) < ErrorTolerance;
}

template <class T>
constexpr bool NearlyEqual(const UE::Math::TVector<T>& Lhs, const UE::Math::TVector<T>& Rhs, T ErrorTolerance)
{
	return NearlyEqual(Lhs.X, Rhs.X, ErrorTolerance) && NearlyEqual(Lhs.Y, Rhs.Y, ErrorTolerance)
		   && NearlyEqual(Lhs.Z, Rhs.Z, ErrorTolerance);
}

template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr bool NearlyEqual(const TValue<LhsUnitType, LhsValueType>& Lhs, const TValue<RhsUnitType, RhsValueType>& Rhs)
{
	return NearlyEqual(Lhs.template GetValue<LhsUnitType>(), Rhs.template GetValue<LhsUnitType>());
}

// Note: comparison operators implemented using value type operators. This means equality comparisons are prone
// to precision errors when using with floats, doubles etc. Use NearlyEqual equality comparison the same way
// you would with built-in types.
template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator==(const TValue<LhsUnitType, LhsValueType>& Lhs, const TValue<RhsUnitType, RhsValueType>& Rhs)
{
	// #TODO_astrosim: taking argument using concept should yield a nicer error message and should be probably more
	// compact, explore.
	static_assert(IsSameDimension(LhsUnitType{}, RhsUnitType{}));
	return Lhs.template GetValue<LhsUnitType>() == Rhs.template GetValue<LhsUnitType>();
}

template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator!=(const TValue<LhsUnitType, LhsValueType>& Lhs, const TValue<RhsUnitType, RhsValueType>& Rhs)
{
	static_assert(IsSameDimension(LhsUnitType{}, RhsUnitType{}));
	return Lhs.template GetValue<LhsUnitType>() != Rhs.template GetValue<LhsUnitType>();
}

template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator<(const TValue<LhsUnitType, LhsValueType>& Lhs, const TValue<RhsUnitType, RhsValueType>& Rhs)
{
	static_assert(IsSameDimension(LhsUnitType{}, RhsUnitType{}));
	return Lhs.template GetValue<LhsUnitType>() < Rhs.template GetValue<LhsUnitType>();
}

template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator<=(const TValue<LhsUnitType, LhsValueType>& Lhs, const TValue<RhsUnitType, RhsValueType>& Rhs)
{
	static_assert(IsSameDimension(LhsUnitType{}, RhsUnitType{}));
	return Lhs.template GetValue<LhsUnitType>() <= Rhs.template GetValue<LhsUnitType>();
}

template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator>(const TValue<LhsUnitType, LhsValueType>& Lhs, const TValue<RhsUnitType, RhsValueType>& Rhs)
{
	static_assert(IsSameDimension(LhsUnitType{}, RhsUnitType{}));
	return Lhs.template GetValue<LhsUnitType>() > Rhs.template GetValue<LhsUnitType>();
}

template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator>=(const TValue<LhsUnitType, LhsValueType>& Lhs, const TValue<RhsUnitType, RhsValueType>& Rhs)
{
	static_assert(IsSameDimension(LhsUnitType{}, RhsUnitType{}));
	return Lhs.template GetValue<LhsUnitType>() >= Rhs.template GetValue<LhsUnitType>();
}

template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator*(const TValue<LhsUnitType, LhsValueType>& Lhs, const TValue<RhsUnitType, RhsValueType>& Rhs)
{
	return TValue{
		LhsUnitType{} * RhsUnitType{}, Lhs.template GetValue<LhsUnitType>() * Rhs.template GetValue<RhsUnitType>()};
}

template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator/(const TValue<LhsUnitType, LhsValueType>& Lhs, const TValue<RhsUnitType, RhsValueType>& Rhs)
{
	return TValue{
		LhsUnitType{} / RhsUnitType{}, Lhs.template GetValue<LhsUnitType>() / Rhs.template GetValue<RhsUnitType>()};
}

template <class LhsUnitType, class RhsUnitType, class ValueType>
constexpr auto operator+(const TValue<LhsUnitType, ValueType>& Lhs, const TValue<RhsUnitType, ValueType>& Rhs)
{
	static_assert(IsSameDimension(LhsUnitType{}, RhsUnitType{}));
	return TValue{LhsUnitType{}, Lhs.template GetValue<LhsUnitType>() + Rhs.template GetValue<LhsUnitType>()};
}

template <class LhsUnitType, class RhsUnitType, class ValueType>
constexpr auto operator-(const TValue<LhsUnitType, ValueType>& Lhs, const TValue<RhsUnitType, ValueType>& Rhs)
{
	static_assert(IsSameDimension(LhsUnitType{}, RhsUnitType{}));
	return Lhs + (-Rhs);
}

template <class UnitType, class ValueType>
constexpr auto operator*(
	const TValue<UnitType, ValueType>& Value, typename TValue<UnitType, ValueType>::ScalarType Scalar)
{
	return TValue{UnitType{}, Value.template GetValue<UnitType>() * Scalar};
}

template <class UnitType, class ValueType>
constexpr auto operator*(
	typename TValue<UnitType, ValueType>::ScalarType Scalar, const TValue<UnitType, ValueType>& Value)
{
	return Value * Scalar;
}

template <class UnitType, class ValueType>
constexpr auto operator/(
	const TValue<UnitType, ValueType>& Value, typename TValue<UnitType, ValueType>::ScalarType Scalar)
{
	return TValue{UnitType{}, Value.template GetValue<UnitType>() / Scalar};
}

}  // namespace SafeMath

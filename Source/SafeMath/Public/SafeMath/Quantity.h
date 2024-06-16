#pragma once

#include "Unit.h"

#include <type_traits>

namespace SafeMath
{

template <class T, class = void>
struct TScalar;

template <class T>
struct TScalar<T, std::enable_if_t<std::is_arithmetic_v<T>, void>>
{
	using Type = T;
};

template <class T>
struct TScalar<UE::Math::TVector<T>>
{
	using Type = typename UE::Math::TVector<T>::FReal;
};

template <class T>
using TScalar_t = typename TScalar<T>::Type;

// #TODO_astrosim: all non-unreal specific things need to be extracted to a third party library for other uses, don't want
// to have to reimplement
template <class InUnitType, class InValueType>
class TQuantity
{
public:
	static_assert(IsUnit(InUnitType{}));
	static_assert(std::is_same_v<std::decay_t<InValueType>, InValueType>);

	using UnitType = InUnitType;
	using ValueType = InValueType;
	using ScalarType = TScalar_t<InValueType>;

	static_assert(IsUnit(UnitType{}));

	explicit constexpr TValue(UnitType, ValueType InValue);

	template <class InTargetUnitType>
	constexpr InValueType GetValue() const
	{
		static_assert(IsUnit(InTargetUnitType{}));
		return GetConversionRateBetween(UnitType{}, InTargetUnitType{}) * Value;
	}

	constexpr TQuantity operator-() const
	{
		return TQuantity{UnitType{}, -Value};
	}

	template <class OtherUnitType>
	TQuantity& operator+=(const TQuantity<OtherUnitType, ValueType>& OtherValue)
	{
		static_assert(IsSameDimension(UnitType{}, OtherUnitType{}));
		Value += OtherValue.template GetValue<UnitType>();
		return *this;
	}

	template <class OtherUnitType>
	TQuantity& operator-=(const TQuantity<OtherUnitType, ValueType>& OtherValue)
	{
		return *this += -OtherValue;
	}

	TQuantity& operator*=(ScalarType Scalar)
	{
		Value *= Scalar;
		return *this;
	}

	TQuantity& operator/=(ScalarType Scalar)
	{
		Value /= Scalar;
		return *this;
	}

private:
	ValueType Value;
};

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

template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr bool NearlyEqual(
	const TQuantity<LhsUnitType, LhsValueType>& Lhs, const TQuantity<RhsUnitType, RhsValueType>& Rhs);

// Note: comparison operators implemented using value type operators. This means equality comparisons are prone
// to precision errors when using with floats, doubles etc. Use NearlyEqual equality comparison the same way
// you would with built-in types.
template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator==(
	const TQuantity<LhsUnitType, LhsValueType>& Lhs, const TQuantity<RhsUnitType, RhsValueType>& Rhs);

template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator!=(
	const TQuantity<LhsUnitType, LhsValueType>& Lhs, const TQuantity<RhsUnitType, RhsValueType>& Rhs);

template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator<(
	const TQuantity<LhsUnitType, LhsValueType>& Lhs, const TQuantity<RhsUnitType, RhsValueType>& Rhs);

template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator<=(
	const TQuantity<LhsUnitType, LhsValueType>& Lhs, const TQuantity<RhsUnitType, RhsValueType>& Rhs);

template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator>(
	const TQuantity<LhsUnitType, LhsValueType>& Lhs, const TQuantity<RhsUnitType, RhsValueType>& Rhs);

template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator>=(
	const TQuantity<LhsUnitType, LhsValueType>& Lhs, const TQuantity<RhsUnitType, RhsValueType>& Rhs);

template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator*(
	const TQuantity<LhsUnitType, LhsValueType>& Lhs, const TQuantity<RhsUnitType, RhsValueType>& Rhs);

template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator/(
	const TQuantity<LhsUnitType, LhsValueType>& Lhs, const TQuantity<RhsUnitType, RhsValueType>& Rhs);

template <class LhsUnitType, class RhsUnitType, class ValueType>
constexpr auto operator+(const TQuantity<LhsUnitType, ValueType>& Lhs, const TQuantity<RhsUnitType, ValueType>& Rhs);

template <class LhsUnitType, class RhsUnitType, class ValueType>
constexpr auto operator-(const TQuantity<LhsUnitType, ValueType>& Lhs, const TQuantity<RhsUnitType, ValueType>& Rhs);

template <class UnitType, class ValueType>
constexpr auto operator*(
	const TQuantity<UnitType, ValueType>& Value, typename TQuantity<UnitType, ValueType>::ScalarType Scalar);

template <class UnitType, class ValueType>
constexpr auto operator*(
	typename TQuantity<UnitType, ValueType>::ScalarType Scalar, const TQuantity<UnitType, ValueType>& Value);

template <class UnitType, class ValueType>
constexpr auto operator/(
	const TQuantity<UnitType, ValueType>& Value, typename TQuantity<UnitType, ValueType>::ScalarType Scalar);

}  // namespace SafeMath

#include "Quantity.inc"

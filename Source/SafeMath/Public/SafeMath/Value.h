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

template <class InUnitType, class InValueType>
class TValue
{
public:
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

	constexpr TValue operator-() const
	{
		return TValue{UnitType{}, -Value};
	}

	template <class OtherUnitType>
	TValue& operator+=(const TValue<OtherUnitType, ValueType>& OtherValue)
	{
		static_assert(IsSameDimension(UnitType{}, OtherUnitType{}));
		Value += OtherValue.template GetValue<UnitType>();
		return *this;
	}

	template <class OtherUnitType>
	TValue& operator-=(const TValue<OtherUnitType, ValueType>& OtherValue)
	{
		return *this += -OtherValue;
	}

	TValue& operator*=(ScalarType Scalar)
	{
		Value *= Scalar;
		return *this;
	}

	TValue& operator/=(ScalarType Scalar)
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
constexpr bool NearlyEqual(const TValue<LhsUnitType, LhsValueType>& Lhs, const TValue<RhsUnitType, RhsValueType>& Rhs);

// Note: comparison operators implemented using value type operators. This means equality comparisons are prone
// to precision errors when using with floats, doubles etc. Use NearlyEqual equality comparison the same way
// you would with built-in types.
template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator==(const TValue<LhsUnitType, LhsValueType>& Lhs, const TValue<RhsUnitType, RhsValueType>& Rhs);

template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator!=(const TValue<LhsUnitType, LhsValueType>& Lhs, const TValue<RhsUnitType, RhsValueType>& Rhs);

template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator<(const TValue<LhsUnitType, LhsValueType>& Lhs, const TValue<RhsUnitType, RhsValueType>& Rhs);

template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator<=(const TValue<LhsUnitType, LhsValueType>& Lhs, const TValue<RhsUnitType, RhsValueType>& Rhs);

template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator>(const TValue<LhsUnitType, LhsValueType>& Lhs, const TValue<RhsUnitType, RhsValueType>& Rhs);

template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator>=(const TValue<LhsUnitType, LhsValueType>& Lhs, const TValue<RhsUnitType, RhsValueType>& Rhs);

template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator*(const TValue<LhsUnitType, LhsValueType>& Lhs, const TValue<RhsUnitType, RhsValueType>& Rhs);

template <class LhsUnitType, class LhsValueType, class RhsUnitType, class RhsValueType>
constexpr auto operator/(const TValue<LhsUnitType, LhsValueType>& Lhs, const TValue<RhsUnitType, RhsValueType>& Rhs);

template <class LhsUnitType, class RhsUnitType, class ValueType>
constexpr auto operator+(const TValue<LhsUnitType, ValueType>& Lhs, const TValue<RhsUnitType, ValueType>& Rhs);

template <class LhsUnitType, class RhsUnitType, class ValueType>
constexpr auto operator-(const TValue<LhsUnitType, ValueType>& Lhs, const TValue<RhsUnitType, ValueType>& Rhs);

template <class UnitType, class ValueType>
constexpr auto operator*(
	const TValue<UnitType, ValueType>& Value, typename TValue<UnitType, ValueType>::ScalarType Scalar);

template <class UnitType, class ValueType>
constexpr auto operator*(
	typename TValue<UnitType, ValueType>::ScalarType Scalar, const TValue<UnitType, ValueType>& Value);

template <class UnitType, class ValueType>
constexpr auto operator/(
	const TValue<UnitType, ValueType>& Value, typename TValue<UnitType, ValueType>::ScalarType Scalar);

}  // namespace SafeMath

#include "Value.inc"

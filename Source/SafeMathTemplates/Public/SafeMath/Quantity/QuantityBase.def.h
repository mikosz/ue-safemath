#pragma once

#include "Quantity.decl.h"
#include "QuantityBase.decl.h"
#include "SafeMath/Quantity/Utils.h"

namespace SafeMath
{

template <class InQuantityType>
template <class InTargetUnitType>
constexpr auto TQuantityBase<InQuantityType>::GetValue() const
{
	static_assert(IsUnit(InTargetUnitType{}));
	using UnitType = typename InQuantityType::UnitType;
	// #TODO_dontcommit: there should be no conversion performed if UnitType is same as target. However, doing this
	// requires equality operator on units, which we don't have atm (but would be trivial to implement, just divide
	// and verify is unitless)
	return GetConversionRateBetween(UnitType{}, InTargetUnitType{}) * static_cast<const InQuantityType&>(*this).Value;
}

template <class LhsQuantityType, class RhsQuantityType>
constexpr bool NearlyEqual(const TQuantityBase<LhsQuantityType>& Lhs, const TQuantityBase<RhsQuantityType>& Rhs)
{
	using LhsUnitType = typename LhsQuantityType::UnitType;
	return NearlyEqual(Lhs.template GetValue<LhsUnitType>(), Rhs.template GetValue<LhsUnitType>());
}

template <class LhsQuantityType, class RhsQuantityType>
constexpr auto operator==(const TQuantityBase<LhsQuantityType>& Lhs, const TQuantityBase<RhsQuantityType>& Rhs)
{
	// #TODO_astrosim: taking argument using concept should yield a nicer error message and should be probably more
	// compact, explore.
	using LhsUnitType = typename LhsQuantityType::UnitType;
	using RhsUnitType = typename RhsQuantityType::UnitType;
	static_assert(IsSameDimension(LhsUnitType{}, RhsUnitType{}));
	return Lhs.template GetValue<LhsUnitType>() == Rhs.template GetValue<LhsUnitType>();
}

template <class LhsQuantityType, class RhsQuantityType>
constexpr auto operator!=(const TQuantityBase<LhsQuantityType>& Lhs, const TQuantityBase<RhsQuantityType>& Rhs)
{
	using LhsUnitType = typename LhsQuantityType::UnitType;
	using RhsUnitType = typename RhsQuantityType::UnitType;
	static_assert(IsSameDimension(LhsUnitType{}, RhsUnitType{}));
	return Lhs.template GetValue<LhsUnitType>() != Rhs.template GetValue<LhsUnitType>();
}

template <class LhsQuantityType, class RhsQuantityType>
constexpr auto operator<(const TQuantityBase<LhsQuantityType>& Lhs, const TQuantityBase<RhsQuantityType>& Rhs)
{
	using LhsUnitType = typename LhsQuantityType::UnitType;
	using RhsUnitType = typename RhsQuantityType::UnitType;
	static_assert(IsSameDimension(LhsUnitType{}, RhsUnitType{}));
	return Lhs.template GetValue<LhsUnitType>() < Rhs.template GetValue<LhsUnitType>();
}

template <class LhsQuantityType, class RhsQuantityType>
constexpr auto operator<=(const TQuantityBase<LhsQuantityType>& Lhs, const TQuantityBase<RhsQuantityType>& Rhs)
{
	using LhsUnitType = typename LhsQuantityType::UnitType;
	using RhsUnitType = typename RhsQuantityType::UnitType;
	static_assert(IsSameDimension(LhsUnitType{}, RhsUnitType{}));
	return Lhs.template GetValue<LhsUnitType>() <= Rhs.template GetValue<LhsUnitType>();
}

template <class LhsQuantityType, class RhsQuantityType>
constexpr auto operator>(const TQuantityBase<LhsQuantityType>& Lhs, const TQuantityBase<RhsQuantityType>& Rhs)
{
	using LhsUnitType = typename LhsQuantityType::UnitType;
	using RhsUnitType = typename RhsQuantityType::UnitType;
	static_assert(IsSameDimension(LhsUnitType{}, RhsUnitType{}));
	return Lhs.template GetValue<LhsUnitType>() > Rhs.template GetValue<LhsUnitType>();
}

template <class LhsQuantityType, class RhsQuantityType>
constexpr auto operator>=(const TQuantityBase<LhsQuantityType>& Lhs, const TQuantityBase<RhsQuantityType>& Rhs)
{
	using LhsUnitType = typename LhsQuantityType::UnitType;
	using RhsUnitType = typename RhsQuantityType::UnitType;
	static_assert(IsSameDimension(LhsUnitType{}, RhsUnitType{}));
	return Lhs.template GetValue<LhsUnitType>() >= Rhs.template GetValue<LhsUnitType>();
}

template <class LhsQuantityType, class RhsQuantityType>
constexpr auto operator*(const TQuantityBase<LhsQuantityType>& Lhs, const TQuantityBase<RhsQuantityType>& Rhs)
{
	using LhsUnitType = typename LhsQuantityType::UnitType;
	using RhsUnitType = typename RhsQuantityType::UnitType;
	return TQuantity{
		LhsUnitType{} * RhsUnitType{}, Lhs.template GetValue<LhsUnitType>() * Rhs.template GetValue<RhsUnitType>()};
}

template <class LhsQuantityType, class RhsQuantityType>
constexpr auto operator/(const TQuantityBase<LhsQuantityType>& Lhs, const TQuantityBase<RhsQuantityType>& Rhs)
{
	using LhsUnitType = typename LhsQuantityType::UnitType;
	using RhsUnitType = typename RhsQuantityType::UnitType;
	return TQuantity{
		LhsUnitType{} / RhsUnitType{}, Lhs.template GetValue<LhsUnitType>() / Rhs.template GetValue<RhsUnitType>()};
}

template <class InQuantityType>
constexpr auto operator-(const TQuantityBase<InQuantityType>& Value){
	using UnitType = typename InQuantityType::UnitType;
	return TQuantity{UnitType{}, -Value.template GetValue<UnitType>()};
}

template <class LhsQuantityType, class RhsQuantityType>
constexpr auto operator+(const TQuantityBase<LhsQuantityType>& Lhs, const TQuantityBase<RhsQuantityType>& Rhs)
{
	using LhsUnitType = typename LhsQuantityType::UnitType;
	using RhsUnitType = typename RhsQuantityType::UnitType;
	static_assert(IsSameDimension(LhsUnitType{}, RhsUnitType{}));
	return TQuantity{LhsUnitType{}, Lhs.template GetValue<LhsUnitType>() + Rhs.template GetValue<LhsUnitType>()};
}

template <class LhsQuantityType, class RhsQuantityType>
constexpr auto operator-(const TQuantityBase<LhsQuantityType>& Lhs, const TQuantityBase<RhsQuantityType>& Rhs)
{
	using LhsUnitType = typename LhsQuantityType::UnitType;
	using RhsUnitType = typename RhsQuantityType::UnitType;
	static_assert(IsSameDimension(LhsUnitType{}, RhsUnitType{}));
	return TQuantity{LhsUnitType{}, Lhs.template GetValue<LhsUnitType>() - Rhs.template GetValue<LhsUnitType>()};
}

template <class InQuantityType>
constexpr auto operator*(const TQuantityBase<InQuantityType>& Value, TScalarType_t<InQuantityType> Scalar)
{
	using UnitType = typename InQuantityType::UnitType;
	return TQuantity{UnitType{}, Value.template GetValue<UnitType>() * Scalar};
}

template <class InQuantityType>
constexpr auto operator*(TScalarType_t<InQuantityType> Scalar, const TQuantityBase<InQuantityType>& Value)
{
	return Value * Scalar;
}

template <class InQuantityType>
constexpr auto operator/(const TQuantityBase<InQuantityType>& Value, TScalarType_t<InQuantityType> Scalar)
{
	using UnitType = typename InQuantityType::UnitType;
	return TQuantity{UnitType{}, Value.template GetValue<UnitType>() / Scalar};
}

}  // namespace SafeMath

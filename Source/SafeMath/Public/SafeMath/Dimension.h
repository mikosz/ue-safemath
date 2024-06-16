#pragma once

#include "SafeMath/Private/ConstexprList.h"

namespace SafeMath
{

namespace BaseDimensions
{

struct FAngle
{
};

struct FDistance
{
};

struct FTime
{
};

struct FMass
{
};

}  // namespace BaseDimensions

template <class InBaseDimensionType, int InExponent>
struct TDimensionEntry
{
	using BaseDimensionType = InBaseDimensionType;

	static constexpr int Exponent = InExponent;

	static constexpr auto TypeMatches = []<class OtherType>(const OtherType&)
	{ return std::is_same_v<BaseDimensionType, typename OtherType::BaseDimensionType>; };
};

// #TODO_astrosim move these two to Private?
template <class T>
consteval bool IsDimensionEntry(T)
{
	return false;
}

template <class BaseDimensionType, int Exponent>
consteval bool IsDimensionEntry(TDimensionEntry<BaseDimensionType, Exponent>)
{
	return true;
}

// #TODO_astrosim document
template <class InBaseDimensionAndExponentListType>
class TDimension
{
	static_assert(Private::ConstexprList::IsConstexprList(InBaseDimensionAndExponentListType{}));
	static_assert(Private::ConstexprList::AllOf(
		InBaseDimensionAndExponentListType{}, [](auto Element) { return IsDimensionEntry(Element); }));

public:
	using BaseDimensionAndExponentListType = InBaseDimensionAndExponentListType;

	TDimension() = default;
};

template <class T>
consteval bool IsDimension(T);

template <class BaseDimensionAndExponentListType>
consteval bool IsDimension(TDimension<BaseDimensionAndExponentListType>);

template <class LhsBaseDimensionAndExponentListType, class RhsBaseDimensionAndExponentListType>
consteval auto operator*(
	TDimension<LhsBaseDimensionAndExponentListType> Lhs, TDimension<RhsBaseDimensionAndExponentListType> Rhs);

template <class BaseDimensionAndExponentListType>
consteval auto Inverse(TDimension<BaseDimensionAndExponentListType> Dimension);

template <class LhsBaseDimensionAndExponentListType, class RhsBaseDimensionAndExponentListType>
consteval auto operator/(
	TDimension<LhsBaseDimensionAndExponentListType> Lhs, TDimension<RhsBaseDimensionAndExponentListType> Rhs);

template <class LhsBaseDimensionAndExponentListType, class RhsBaseDimensionAndExponentListType>
consteval bool operator==(
	TDimension<LhsBaseDimensionAndExponentListType> Lhs, TDimension<RhsBaseDimensionAndExponentListType> Rhs);

template <class LhsBaseDimensionAndExponentListType, class RhsBaseDimensionAndExponentListType>
consteval bool operator!=(
	TDimension<LhsBaseDimensionAndExponentListType> Lhs, TDimension<RhsBaseDimensionAndExponentListType> Rhs);

template <class BaseDimensionType, int Exponent>
consteval auto MakeDimension();

}  // namespace SafeMath

#include "Dimension.inc"

namespace SafeMath::Dimensions
{

using FAngle = decltype(MakeDimension<BaseDimensions::FAngle, 1>());
using FDistance = decltype(MakeDimension<BaseDimensions::FDistance, 1>());
using FTime = decltype(MakeDimension<BaseDimensions::FTime, 1>());
using FMass = decltype(MakeDimension<BaseDimensions::FMass, 1>());

using FVelocity = decltype(FDistance{} / FTime{});
using FMomentum = decltype(FMass{} * FVelocity{});
using FAcceleration = decltype(FVelocity{} / FTime{});
using FForce = decltype(FMass{} * FAcceleration{});
using FEnergy = decltype(FForce{} * FDistance{});
using FPower = decltype(FEnergy{} / FTime{});

}  // namespace SafeMath::Dimensions

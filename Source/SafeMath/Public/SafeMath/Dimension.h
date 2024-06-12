#pragma once

#include "SafeMath/Private/ConstexprList.h"

namespace SafeMath
{

namespace BaseDimensions
{

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

template <class T>
constexpr bool IsDimensionEntry(T)
{
	return false;
}

template <class BaseDimensionType, int Exponent>
constexpr bool IsDimensionEntry(TDimensionEntry<BaseDimensionType, Exponent>)
{
	return true;
}

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

template <class LhsBaseDimensionAndExponentListType, class RhsBaseDimensionAndExponentListType>
constexpr auto operator*(
	TDimension<LhsBaseDimensionAndExponentListType> Lhs, TDimension<RhsBaseDimensionAndExponentListType> Rhs);

template <class BaseDimensionAndExponentListType>
constexpr auto Inverse(TDimension<BaseDimensionAndExponentListType> Dimension);

template <class LhsBaseDimensionAndExponentListType, class RhsBaseDimensionAndExponentListType>
constexpr auto operator/(
	TDimension<LhsBaseDimensionAndExponentListType> Lhs, TDimension<RhsBaseDimensionAndExponentListType> Rhs);

template <class LhsBaseDimensionAndExponentListType, class RhsBaseDimensionAndExponentListType>
constexpr bool operator==(TDimension<LhsBaseDimensionAndExponentListType> Lhs, TDimension<RhsBaseDimensionAndExponentListType> Rhs);

template <class LhsBaseDimensionAndExponentListType, class RhsBaseDimensionAndExponentListType>
constexpr bool operator!=(TDimension<LhsBaseDimensionAndExponentListType> Lhs, TDimension<RhsBaseDimensionAndExponentListType> Rhs);

template <class BaseDimensionType, int Exponent>
constexpr auto MakeDimension()
{
	return TDimension<Private::TConstexprList<TDimensionEntry<BaseDimensionType, Exponent>>>{};
}

namespace Dimensions
{

using FDistance = decltype(MakeDimension<BaseDimensions::FDistance, 1>());
using FTime = decltype(MakeDimension<BaseDimensions::FTime, 1>());
using FMass = decltype(MakeDimension<BaseDimensions::FMass, 1>());

}  // namespace Dimensions

}  // namespace SafeMath

#include "Dimension.inc"

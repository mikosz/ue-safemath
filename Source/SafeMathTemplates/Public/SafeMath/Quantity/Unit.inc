#pragma once

#include "Unit.h"

namespace SafeMath
{

template <class T>
consteval bool IsUnit(T)
{
	return false;
}

template <class InDimensionType, double InConversionRate>
consteval bool IsUnit(TUnit<InDimensionType, InConversionRate>)
{
	return true;
}

template <class LhsDimensionType, double LhsConversionRate, class RhsDimensionType, double RhsConversionRate>
consteval bool IsSameDimension(
	TUnit<LhsDimensionType, LhsConversionRate> Lhs, TUnit<RhsDimensionType, RhsConversionRate> Rhs)
{
	return LhsDimensionType{} == RhsDimensionType{};
}

template <class FromDimensionType, double FromConversionRate, class ToDimensionType, double ToConversionRate>
consteval double GetConversionRateBetween(
	TUnit<FromDimensionType, FromConversionRate> From, TUnit<ToDimensionType, ToConversionRate> To)
{
	static_assert(IsSameDimension(From, To));
	return FromConversionRate / ToConversionRate;
}

template <class LhsDimensionType, double LhsConversionRate, class RhsDimensionType, double RhsConversionRate>
consteval auto operator*(TUnit<LhsDimensionType, LhsConversionRate> Lhs, TUnit<RhsDimensionType, RhsConversionRate> Rhs)
{
	return TUnit<decltype(LhsDimensionType{} * RhsDimensionType{}), LhsConversionRate * RhsConversionRate>{};
}

template <class LhsDimensionType, double LhsConversionRate, class RhsDimensionType, double RhsConversionRate>
consteval auto operator/(TUnit<LhsDimensionType, LhsConversionRate> Lhs, TUnit<RhsDimensionType, RhsConversionRate> Rhs)
{
	return TUnit<decltype(LhsDimensionType{} / RhsDimensionType{}), LhsConversionRate / RhsConversionRate>{};
}

}  // namespace SafeMath

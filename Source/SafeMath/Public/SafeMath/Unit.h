#pragma once

#include "Dimension.h"

namespace SafeMath
{

template <class InDimensionType, double InConversionRate>
class TUnit
{
public:
	static_assert(IsDimension(InDimensionType{}));
	static_assert(InConversionRate > UE_DOUBLE_SMALL_NUMBER);

	using DimensionType = InDimensionType;

	static constexpr auto ConversionRate = InConversionRate;
};

template <class LhsDimensionType, double LhsConversionRate, class RhsDimensionType, double RhsConversionRate>
constexpr bool IsSameDimension(
	TUnit<LhsDimensionType, LhsConversionRate> Lhs, TUnit<RhsDimensionType, RhsConversionRate> Rhs)
{
	return LhsDimensionType{} == RhsDimensionType{};
}

template <class FromDimensionType, double FromConversionRate, class ToDimensionType, double ToConversionRate>
constexpr double GetConversionRateBetween(
	TUnit<FromDimensionType, FromConversionRate> From, TUnit<ToDimensionType, ToConversionRate> To)
{
	static_assert(IsSameDimension(From, To));
	return FromConversionRate / ToConversionRate;
}

template <class LhsDimensionType, double LhsConversionRate, class RhsDimensionType, double RhsConversionRate>
constexpr auto operator*(TUnit<LhsDimensionType, LhsConversionRate> Lhs, TUnit<RhsDimensionType, RhsConversionRate> Rhs)
{
	return TUnit<decltype(LhsDimensionType{} * RhsDimensionType{}), LhsConversionRate * RhsConversionRate>{};
}

template <class LhsDimensionType, double LhsConversionRate, class RhsDimensionType, double RhsConversionRate>
constexpr auto operator/(TUnit<LhsDimensionType, LhsConversionRate> Lhs, TUnit<RhsDimensionType, RhsConversionRate> Rhs)
{
	return TUnit<decltype(LhsDimensionType{} / RhsDimensionType{}), LhsConversionRate / RhsConversionRate>{};
}

namespace Units
{

using FDegrees = TUnit<Dimensions::FAngle, 1.>;
using FRadians = TUnit<Dimensions::FAngle, 180. / UE_DOUBLE_PI>;

using FKilograms = TUnit<Dimensions::FMass, 1.>;

using FCentimeters = TUnit<Dimensions::FDistance, .01>;
using FMeters = TUnit<Dimensions::FDistance, 1.>;
using FKilometers = TUnit<Dimensions::FDistance, 1000.>;

using FSeconds = TUnit<Dimensions::FTime, 1.>;
using FHours = TUnit<Dimensions::FTime, 60. * 60.>;

using FMetersPerSecond = decltype(FMeters{} / FSeconds{});
using FKilometersPerHour = decltype(FKilometers{} / FHours{});

using FMetersPerSecondPerSecond = decltype(FMeters{} / FSeconds{} / FSeconds{});

}  // namespace Units

}  // namespace SafeMath

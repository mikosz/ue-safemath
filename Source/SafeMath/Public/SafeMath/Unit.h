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

template <class T>
consteval bool IsUnit(T);

template <class InDimensionType, double InConversionRate>
consteval bool IsUnit(TUnit<InDimensionType, InConversionRate>);

template <class LhsDimensionType, double LhsConversionRate, class RhsDimensionType, double RhsConversionRate>
consteval bool IsSameDimension(
	TUnit<LhsDimensionType, LhsConversionRate> Lhs, TUnit<RhsDimensionType, RhsConversionRate> Rhs);

template <class FromDimensionType, double FromConversionRate, class ToDimensionType, double ToConversionRate>
consteval double GetConversionRateBetween(
	TUnit<FromDimensionType, FromConversionRate> From, TUnit<ToDimensionType, ToConversionRate> To);

template <class LhsDimensionType, double LhsConversionRate, class RhsDimensionType, double RhsConversionRate>
consteval auto operator*(
	TUnit<LhsDimensionType, LhsConversionRate> Lhs, TUnit<RhsDimensionType, RhsConversionRate> Rhs);

template <class LhsDimensionType, double LhsConversionRate, class RhsDimensionType, double RhsConversionRate>
consteval auto operator/(
	TUnit<LhsDimensionType, LhsConversionRate> Lhs, TUnit<RhsDimensionType, RhsConversionRate> Rhs);

}  // namespace SafeMath

#include "Unit.inc"

namespace SafeMath::Units
{

using FDegrees = TUnit<Dimensions::FAngle, 1.>;
using FRadians = TUnit<Dimensions::FAngle, 180. / UE_DOUBLE_PI>;

using FGrams = TUnit<Dimensions::FMass, .001>;
using FKilograms = TUnit<Dimensions::FMass, 1.>;

using FCentimeters = TUnit<Dimensions::FDistance, .01>;
using FMeters = TUnit<Dimensions::FDistance, 1.>;
using FKilometers = TUnit<Dimensions::FDistance, 1000.>;

using FCentimetersSquared = decltype(FCentimeters{} * FCentimeters{});
using FMetersSquared = decltype(FMeters{} * FMeters{});
using FKilometersSquared = decltype(FKilometers{} * FKilometers{});

using FSeconds = TUnit<Dimensions::FTime, 1.>;
using FHours = TUnit<Dimensions::FTime, 60. * 60.>;

using FMetersPerSecond = decltype(FMeters{} / FSeconds{});
using FKilometersPerHour = decltype(FKilometers{} / FHours{});

using FMetersPerSecondPerSecond = decltype(FMeters{} / FSeconds{} / FSeconds{});

}  // namespace SafeMath::Units

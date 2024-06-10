#pragma once

namespace SafeMath
{

namespace Dimensions
{

template <class T, int Exponent>
class TBaseDimension final
{
};

struct FBaseDistance
{
};

struct FBaseTime
{
};

struct FBaseAngle
{
};

struct FBaseMass
{
};

using FDistance = TBaseDimension<FBaseDistance, 1>;
using FArea = TBaseDimension<FBaseDistance, 2>;
using FTime = TBaseDimension<FBaseTime, 1>;
using FAngle = TBaseDimension<FBaseAngle, 1>;
using FMass = TBaseDimension<FBaseMass, 1>;

}  // namespace Dimensions

template <class BaseDimensionTypeList>
class TDimension final
{
public:
	constexpr auto Inverse() const;

	template <class OtherDimensionTypeList>
	constexpr auto operator*(TDimension<OtherDimensionTypeList> OtherDimension) const;

	template <class OtherDimensionTypeList>
	constexpr auto operator/(TDimension<OtherDimensionTypeList> OtherDimension) const;

private:
	constexpr auto Simplify() const;
};

struct FDistance
{
};

struct FTime
{
};

struct FAngle
{
};

struct FMass
{
};

template <class BaseDimensionType, int Exponent>
constexpr auto MakeDimension();

}  // namespace SafeMath

#include "Dimension.inc"

#pragma once

// #TODO_dontcommit put in other file (probably separate utility plugin as another submodule like errant utilities)

namespace SafeMath
{

template <class T>
struct TSmallNumber;

template <>
struct TSmallNumber<double>
{
	static constexpr double Value = UE_DOUBLE_SMALL_NUMBER;
};

template <>
struct TSmallNumber<float>
{
	static constexpr double Value = UE_SMALL_NUMBER;
};

template <class T>
constexpr T TSmallNumber_v = TSmallNumber<T>::Value;

template <class T>
bool IsNearlyEqual(const T& LHS, const T& RHS)
{
	if constexpr (std::is_floating_point_v<T>)
	{
		return FMath::IsNearlyEqual(LHS, RHS, TSmallNumber_v<T>);
	}
	else
	{
		return LHS == RHS;
	}
}

}  // namespace SafeMath

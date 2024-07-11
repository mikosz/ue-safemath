#pragma once

namespace SafeMath
{

template <class T>
constexpr std::enable_if_t<std::is_floating_point_v<T>, T> Abs(T Value)
{
	return Value < T{0} ? -Value : Value;
}

template <class T>
constexpr std::enable_if_t<std::is_floating_point_v<T>, bool> NearlyEqual(T Lhs, T Rhs, T ErrorTolerance)
{
	return Abs(Lhs - Rhs) < ErrorTolerance;
}

template <class T>
constexpr bool NearlyEqual(const UE::Math::TVector<T>& Lhs, const UE::Math::TVector<T>& Rhs, T ErrorTolerance)
{
	return NearlyEqual(Lhs.X, Rhs.X, ErrorTolerance) && NearlyEqual(Lhs.Y, Rhs.Y, ErrorTolerance)
		   && NearlyEqual(Lhs.Z, Rhs.Z, ErrorTolerance);
}

}  // namespace SafeMath

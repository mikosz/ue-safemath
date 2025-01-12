#pragma once

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
using TScalar_t = typename TScalar<std::decay_t<T>>::Type;

template <class InQuantityType>
class TQuantityBase
{
public:
	using QuantityType = InQuantityType;

	template <class InTargetUnitType>
	constexpr auto GetValue() const;
};

template <class InQuantityType>
struct TUnitType
{
	using Type = typename InQuantityType::UnitType;
};

template <class T>
using TUnitType_t = typename TUnitType<T>::Type;

template <class InQuantityType>
struct TValueType
{
	using Type = typename InQuantityType::ValueType;
};

template <class T>
using TValueType_t = typename TValueType<T>::Type;

template <class InQuantityType>
struct TScalarType
{
	using Type = typename InQuantityType::ScalarType;
};

template <class T>
using TScalarType_t = typename TScalarType<T>::Type;

template <class LhsQuantityType, class RhsQuantityType>
constexpr bool NearlyEqual(const TQuantityBase<LhsQuantityType>& Lhs, const TQuantityBase<RhsQuantityType>& Rhs);

// Note: comparison operators implemented using value type operators. This means equality comparisons are prone
// to precision errors when using with floats, doubles etc. Use NearlyEqual equality comparison the same way
// you would with built-in types.
template <class LhsQuantityType, class RhsQuantityType>
constexpr auto operator==(const TQuantityBase<LhsQuantityType>& Lhs, const TQuantityBase<RhsQuantityType>& Rhs);

template <class LhsQuantityType, class RhsQuantityType>
constexpr auto operator!=(const TQuantityBase<LhsQuantityType>& Lhs, const TQuantityBase<RhsQuantityType>& Rhs);

template <class LhsQuantityType, class RhsQuantityType>
constexpr auto operator<(const TQuantityBase<LhsQuantityType>& Lhs, const TQuantityBase<RhsQuantityType>& Rhs);

template <class LhsQuantityType, class RhsQuantityType>
constexpr auto operator<=(const TQuantityBase<LhsQuantityType>& Lhs, const TQuantityBase<RhsQuantityType>& Rhs);

template <class LhsQuantityType, class RhsQuantityType>
constexpr auto operator>(const TQuantityBase<LhsQuantityType>& Lhs, const TQuantityBase<RhsQuantityType>& Rhs);

template <class LhsQuantityType, class RhsQuantityType>
constexpr auto operator>=(const TQuantityBase<LhsQuantityType>& Lhs, const TQuantityBase<RhsQuantityType>& Rhs);

template <class LhsQuantityType, class RhsQuantityType>
constexpr auto operator*(const TQuantityBase<LhsQuantityType>& Lhs, const TQuantityBase<RhsQuantityType>& Rhs);

template <class LhsQuantityType, class RhsQuantityType>
constexpr auto operator/(const TQuantityBase<LhsQuantityType>& Lhs, const TQuantityBase<RhsQuantityType>& Rhs);

template <class InQuantityType>
constexpr auto operator-(const TQuantityBase<InQuantityType>& Value);

template <class LhsQuantityType, class RhsQuantityType>
constexpr auto operator+(const TQuantityBase<LhsQuantityType>& Lhs, const TQuantityBase<RhsQuantityType>& Rhs);

template <class LhsQuantityType, class RhsQuantityType>
constexpr auto operator-(const TQuantityBase<LhsQuantityType>& Lhs, const TQuantityBase<RhsQuantityType>& Rhs);

template <class InQuantityType>
constexpr auto operator*(const TQuantityBase<InQuantityType>& Value, TScalarType_t<InQuantityType> Scalar);

template <class InQuantityType>
constexpr auto operator*(TScalarType_t<InQuantityType> Scalar, const TQuantityBase<InQuantityType>& Value);

template <class InQuantityType>
constexpr auto operator/(const TQuantityBase<InQuantityType>& Value, TScalarType_t<InQuantityType> Scalar);

}  // namespace SafeMath

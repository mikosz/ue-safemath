#pragma once

#include "QuantityBase.decl.h"

namespace SafeMath
{

// #TODO_astrosim: all non-unreal specific things need to be extracted to a third party library for other uses, don't want
// to have to reimplement
template <class InUnitType, class InValueType>
class TQuantity : public TQuantityBase<TQuantity<InUnitType, InValueType>>
{
public:
	static_assert(IsUnit(InUnitType{}));
	static_assert(std::is_same_v<std::decay_t<InValueType>, InValueType>);

	using UnitType = InUnitType;
	using ValueType = InValueType;
	using ScalarType = TScalar_t<InValueType>;

	// #TODO_dontcommit: make either guides or constructors to convert references (lvalue and rvalue) to value when deducing type, add tests
	explicit constexpr TQuantity(UnitType, ValueType InValue);

	// #TODO_dontcommit: make TQuantity implicitly convertible from other quantities of the same type, potentially
	// also for compatible value types (e.g. float -> double).
	// Should have also explicit conversion for explicitly convertible value types.

	template <class InTargetUnitType>
	constexpr InValueType GetValue() const;

	template <class OtherUnitType>
	TQuantity& operator+=(const TQuantity<OtherUnitType, ValueType>& OtherValue);

	template <class OtherUnitType>
	TQuantity& operator-=(const TQuantity<OtherUnitType, ValueType>& OtherValue);

	TQuantity& operator*=(ScalarType Scalar);

	TQuantity& operator/=(ScalarType Scalar);

	ValueType* Data();

	const ValueType* Data() const;

private:
	ValueType Value;

	friend class TQuantityBase<TQuantity>;
};

template <class InUnitType, class InValueType>
TQuantity(InUnitType, InValueType) -> TQuantity<InUnitType, std::decay_t<InValueType>>;

}  // namespace SafeMath

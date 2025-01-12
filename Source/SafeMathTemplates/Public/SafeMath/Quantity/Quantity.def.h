#pragma once

#include "Quantity.decl.h"
#include "QuantityBase.def.h"

namespace SafeMath
{

template <class InUnitType, class InValueType>
constexpr TQuantity<InUnitType, InValueType>::TQuantity(UnitType, ValueType InValue) : Value{InValue}
{
}

template <class InUnitType, class InValueType>
template <class InTargetUnitType>
constexpr InValueType TQuantity<InUnitType, InValueType>::GetValue() const
{
	static_assert(IsUnit(InTargetUnitType{}));
	return GetConversionRateBetween(UnitType{}, InTargetUnitType{}) * Value;
}

template <class InUnitType, class InValueType>
template <class OtherUnitType>
TQuantity<InUnitType, InValueType>& TQuantity<InUnitType, InValueType>::operator+=(
	const TQuantity<OtherUnitType, ValueType>& OtherValue)
{
	static_assert(IsSameDimension(UnitType{}, OtherUnitType{}));
	Value += OtherValue.template GetValue<UnitType>();
	return *this;
}

template <class InUnitType, class InValueType>
template <class OtherUnitType>
TQuantity<InUnitType, InValueType>& TQuantity<InUnitType, InValueType>::operator-=(
	const TQuantity<OtherUnitType, ValueType>& OtherValue)
{
	return *this += -OtherValue;
}

template <class InUnitType, class InValueType>
TQuantity<InUnitType, InValueType>& TQuantity<InUnitType, InValueType>::operator*=(ScalarType Scalar)
{
	Value *= Scalar;
	return *this;
}

template <class InUnitType, class InValueType>
TQuantity<InUnitType, InValueType>& TQuantity<InUnitType, InValueType>::operator/=(ScalarType Scalar)
{
	Value /= Scalar;
	return *this;
}

// #TODO_dontcommit want trailing return type but gets converted, look at settings (potentially create new layer
// for private style, but don't want to overwrite errant)
template <class InUnitType, class InValueType>
typename TQuantity<InUnitType, InValueType>::ValueType* TQuantity<InUnitType, InValueType>::Data()
{
	return &Value;
}

template <class InUnitType, class InValueType>
const typename TQuantity<InUnitType, InValueType>::ValueType* TQuantity<InUnitType, InValueType>::Data() const
{
	return &Value;
}

}  // namespace SafeMath

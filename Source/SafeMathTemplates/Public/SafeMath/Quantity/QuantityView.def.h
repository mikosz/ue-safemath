#pragma once

#include "QuantityBase.def.h"
#include "QuantityView.decl.h"

namespace SafeMath
{

template <class InUnitType, class InViewedValueType>
constexpr TQuantityView<InUnitType, InViewedValueType>::TQuantityView(UnitType, ReferenceType InReference)
	: Value{InReference}
{
}

template <class InUnitType, class InViewedValueType>
constexpr TQuantityView<InUnitType, InViewedValueType>::TQuantityView(TQuantity<UnitType, ValueType>& QuantityReference)
	: Value{QuantityReference.Value}
{
}

template <class InUnitType, class InViewedValueType>
constexpr TQuantityView<InUnitType, InViewedValueType>::TQuantityView(
	const TQuantity<UnitType, ValueType>& QuantityReference)
	: Value{QuantityReference.Value}
{
}

template <class InUnitType, class InViewedValueType>
template <class InTargetUnitType>
// ReSharper disable once CppEnforceFunctionDeclarationStyle
constexpr auto TQuantityView<InUnitType, InViewedValueType>::GetValue() const -> ValueType
{
	static_assert(IsUnit(InTargetUnitType{}));
	return ToQuantity().template GetValue<InTargetUnitType>();
}

template <class InUnitType, class InViewedValueType>
// ReSharper disable once CppEnforceFunctionDeclarationStyle
constexpr auto TQuantityView<InUnitType, InViewedValueType>::ToQuantity() const -> TQuantity<UnitType, ValueType>
{
	return TQuantity{UnitType{}, Value};
}

template <class InUnitType, class InViewedValueType>
template <class TargetUnitType, class TargetValueType>
TQuantityView<InUnitType, InViewedValueType>::operator TQuantity<TargetUnitType, TargetValueType>() const
{
	return ToQuantity();
}

template <class InUnitType, class InViewedValueType>
TQuantityView<InUnitType, InViewedValueType>& TQuantityView<InUnitType, InViewedValueType>::operator*=(
	ScalarType Scalar)
{
	Value *= Scalar;
	return *this;
}

template <class InUnitType, class InViewedValueType>
TQuantityView<InUnitType, InViewedValueType>& TQuantityView<InUnitType, InViewedValueType>::operator/=(
	ScalarType Scalar)
{
	Value /= Scalar;
	return *this;
}

}  // namespace SafeMath

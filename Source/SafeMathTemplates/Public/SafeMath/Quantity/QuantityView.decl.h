#pragma once

#include "QuantityBase.h"

namespace SafeMath
{

template <class InUnitType, class InViewedValueType>
class TQuantityView : public TQuantityBase<TQuantityView<InUnitType, InViewedValueType>>
{
public:
	static_assert(IsUnit(InUnitType{}));

	using UnitType = InUnitType;
	using ValueType = std::decay_t<InViewedValueType>;
	using ReferenceType = InViewedValueType&;
	using ScalarType = TScalar_t<ValueType>;

	explicit constexpr TQuantityView(UnitType, InViewedValueType& InReference);
	// ReSharper disable once CppNonExplicitConvertingConstructor
	constexpr TQuantityView(TQuantity<UnitType, ValueType>& QuantityReference);
	// ReSharper disable once CppNonExplicitConvertingConstructor
	constexpr TQuantityView(const TQuantity<UnitType, ValueType>& QuantityReference);

	template <class InTargetUnitType>
	constexpr ValueType GetValue() const;

	constexpr TQuantity<UnitType, ValueType> ToQuantity() const;

	template <class TargetUnitType, class TargetValueType>
	// ReSharper disable once CppNonExplicitConversionOperator
	operator TQuantity<TargetUnitType, TargetValueType>() const;

	template <class QuantityType>
	std::enable_if_t<std::is_convertible_v<QuantityType, TQuantity<UnitType, ValueType>>, TQuantityView&> operator+=(
		const QuantityType& Quantity)
	{
		Value += Quantity.template GetValue<UnitType>();
		return *this;
	}

	template <class QuantityType>
	std::enable_if_t<std::is_convertible_v<QuantityType, TQuantity<UnitType, ValueType>>, TQuantityView&> operator-=(
		const QuantityType& Quantity)
	{
		Value -= Quantity.template GetValue<UnitType>();
		return *this;
	}

	TQuantityView& operator*=(ScalarType Scalar);

	TQuantityView& operator/=(ScalarType Scalar);

private:
	ReferenceType Value;

	friend class TQuantityBase<TQuantityView>;
};

}  // namespace SafeMath

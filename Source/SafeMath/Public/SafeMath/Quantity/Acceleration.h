

// #TODO_dontcommit: add copyright everywhere (to file template also)
#pragma once

// #TODO_dontcommit: unreal types should be in a separate plugin I think. Others could just be in a third-party project

// #TODO_dontcommit: dimensions must be blueprint editable

#include "SetSelectedUnitStrategy.h"
#include "SafeMath/Quantity/Quantity.h"
#include "SafeMath/Quantity/Unit.h"

#include <variant>

#include "Acceleration.generated.h"

UENUM()
enum class EAccelerationUnit : uint8
{
    
    MetersPerSecondPerSecond UMETA(DisplayName="m/s^2"),
    
};

template <class AccelerationUnit>
struct FAccelerationUnitTraits;


template <>
struct FAccelerationUnitTraits<SafeMath::Units::FMetersPerSecondPerSecond>
{
	constexpr static EAccelerationUnit AccelerationUnit = EAccelerationUnit::MetersPerSecondPerSecond;

	inline static const TCHAR* Suffix = TEXT("m/s^2");
};


using FAccelerationUnit = std::variant<

    SafeMath::Units::FMetersPerSecondPerSecond

>;

constexpr FAccelerationUnit GetUnitVariant(const EAccelerationUnit AccelerationUnit)
{
	switch (AccelerationUnit)
	{

        case EAccelerationUnit::MetersPerSecondPerSecond:
			return FAccelerationUnit{ SafeMath::Units::FMetersPerSecondPerSecond{} };

        default:
            return FAccelerationUnit{ SafeMath::Units::FMetersPerSecondPerSecond{} };
	}
}

USTRUCT()
struct FAcceleration 
{
	GENERATED_BODY()

	using DefaultUnitType = SafeMath::Units::FMetersPerSecondPerSecond;

    using NumericType = double;

	using QuantityType = SafeMath::TQuantity<DefaultUnitType, NumericType>;

	using UnitEnumType = EAccelerationUnit;

	static FName GetValuePropertyName()
	{
		return GET_MEMBER_NAME_CHECKED(FAcceleration, Value);
	}

	static FName GetSelectedUnitPropertyName()
	{
		return GET_MEMBER_NAME_CHECKED(FAcceleration, SelectedUnit);
	}

	FAcceleration() = default;

	template <class CompatibleUnitType>
	FAcceleration(SafeMath::TQuantity<CompatibleUnitType, NumericType> Quantity)
		: Value(Quantity.template GetValue<DefaultUnitType>())
#if WITH_EDITORONLY_DATA
		, SelectedUnit(FAccelerationUnitTraits<CompatibleUnitType>::AccelerationUnit)
#endif
	{
		static_assert(SafeMath::IsSameDimension(DefaultUnitType{}, CompatibleUnitType{}));
	}

	operator QuantityType() const
	{
		return GetQuantity();
	}

	QuantityType GetQuantity() const
	{
		return QuantityType{DefaultUnitType{}, Value};
	}

    template <class UnitType>
    NumericType GetValue() const
    {
        return GetQuantity().GetValue<UnitType>();
    }

#if WITH_EDITORONLY_DATA
	EAccelerationUnit GetSelectedUnit() const
	{
		return SelectedUnit;
	}

	void SetSelectedUnit(EAccelerationUnit InSelectedUnit, ESetSelectedUnitStrategy SetSelectedUnitStrategy)
	{
		if (SetSelectedUnitStrategy == ESetSelectedUnitStrategy::KeepNumericalValue)
		{
			const auto FromUnitVariant = GetUnitVariant(SelectedUnit);
			const auto ToUnitVariant = GetUnitVariant(InSelectedUnit);

			std::visit(
				[this](auto FromUnit, auto ToUnit)
				{ Value = SafeMath::GetConversionRateBetween(ToUnit, FromUnit) * Value; },
				FromUnitVariant,
				ToUnitVariant);
		}

		SelectedUnit = InSelectedUnit;
	}
#endif

	FAcceleration& operator*=(const NumericType Scalar)
	{
		Value *= Scalar;
		return *this;
	}

	FAcceleration& operator/=(const NumericType Scalar)
	{
		Value /= Scalar;
		return *this;
	}

private:
	// Always internally stored in default dimension unit (conversion rate == 1)
	UPROPERTY(EditAnywhere)
	double Value = 0.;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere)
	EAccelerationUnit SelectedUnit = FAccelerationUnitTraits<DefaultUnitType>::AccelerationUnit;
#endif
};

inline FAcceleration operator*(const FAcceleration Acceleration, const double Scalar)
{
	return FAcceleration{ Acceleration } *= Scalar;
}

inline FAcceleration operator*(const double Scalar, const FAcceleration Acceleration)
{
	return Acceleration * Scalar;
}

inline FAcceleration operator/(const FAcceleration Acceleration, const double Scalar)
{
	return FAcceleration{ Acceleration } /= Scalar;
}

inline FAcceleration operator/(const double Scalar, const FAcceleration Acceleration)
{
	return Acceleration / Scalar;
}

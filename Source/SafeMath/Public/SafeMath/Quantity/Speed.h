

// #TODO_dontcommit: add copyright everywhere (to file template also)
#pragma once

// #TODO_dontcommit: unreal types should be in a separate plugin I think. Others could just be in a third-party project

// #TODO_dontcommit: dimensions must be blueprint editable

#include "SetSelectedUnitStrategy.h"
#include "SafeMath/Quantity/Quantity.h"
#include "SafeMath/Quantity/Unit.h"

#include <variant>

#include "Speed.generated.h"

UENUM()
enum class ESpeedUnit : uint8
{
    
    MetersPerSecond UMETA(DisplayName="m/s"),
    
    KilometersPerHour UMETA(DisplayName="km/h"),
    
};

template <class SpeedUnit>
struct FSpeedUnitTraits;


template <>
struct FSpeedUnitTraits<SafeMath::Units::FMetersPerSecond>
{
	constexpr static ESpeedUnit SpeedUnit = ESpeedUnit::MetersPerSecond;

	inline static const TCHAR* Suffix = TEXT("m/s");
};

template <>
struct FSpeedUnitTraits<SafeMath::Units::FKilometersPerHour>
{
	constexpr static ESpeedUnit SpeedUnit = ESpeedUnit::KilometersPerHour;

	inline static const TCHAR* Suffix = TEXT("km/h");
};


using FSpeedUnit = std::variant<

    SafeMath::Units::FMetersPerSecond,

    SafeMath::Units::FKilometersPerHour

>;

constexpr FSpeedUnit GetUnitVariant(const ESpeedUnit SpeedUnit)
{
	switch (SpeedUnit)
	{

        case ESpeedUnit::MetersPerSecond:
			return FSpeedUnit{ SafeMath::Units::FMetersPerSecond{} };

        case ESpeedUnit::KilometersPerHour:
			return FSpeedUnit{ SafeMath::Units::FKilometersPerHour{} };

        default:
            return FSpeedUnit{ SafeMath::Units::FMetersPerSecond{} };
	}
}

USTRUCT()
struct FSpeed 
{
	GENERATED_BODY()

	using DefaultUnitType = SafeMath::Units::FMetersPerSecond;

    using NumericType = double;

	using QuantityType = SafeMath::TQuantity<DefaultUnitType, NumericType>;

	using UnitEnumType = ESpeedUnit;

	static FName GetValuePropertyName()
	{
		return GET_MEMBER_NAME_CHECKED(FSpeed, Value);
	}

	static FName GetSelectedUnitPropertyName()
	{
		return GET_MEMBER_NAME_CHECKED(FSpeed, SelectedUnit);
	}

	FSpeed() = default;

	template <class CompatibleUnitType>
	FSpeed(SafeMath::TQuantity<CompatibleUnitType, NumericType> Quantity)
		: Value(Quantity.template GetValue<DefaultUnitType>())
#if WITH_EDITORONLY_DATA
		, SelectedUnit(FSpeedUnitTraits<CompatibleUnitType>::SpeedUnit)
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
	ESpeedUnit GetSelectedUnit() const
	{
		return SelectedUnit;
	}

	void SetSelectedUnit(ESpeedUnit InSelectedUnit, ESetSelectedUnitStrategy SetSelectedUnitStrategy)
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

	FSpeed& operator*=(const NumericType Scalar)
	{
		Value *= Scalar;
		return *this;
	}

	FSpeed& operator/=(const NumericType Scalar)
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
	ESpeedUnit SelectedUnit = FSpeedUnitTraits<DefaultUnitType>::SpeedUnit;
#endif
};

inline FSpeed operator*(const FSpeed Speed, const double Scalar)
{
	return FSpeed{ Speed } *= Scalar;
}

inline FSpeed operator*(const double Scalar, const FSpeed Speed)
{
	return Speed * Scalar;
}

inline FSpeed operator/(const FSpeed Speed, const double Scalar)
{
	return FSpeed{ Speed } /= Scalar;
}

inline FSpeed operator/(const double Scalar, const FSpeed Speed)
{
	return Speed / Scalar;
}

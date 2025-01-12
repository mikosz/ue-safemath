

// #TODO_dontcommit: add copyright everywhere (to file template also)
#pragma once

// #TODO_dontcommit: unreal types should be in a separate plugin I think. Others could just be in a third-party project

// #TODO_dontcommit: dimensions must be blueprint editable

#include "SetSelectedUnitStrategy.h"
#include "SafeMath/Quantity/Quantity.h"
#include "SafeMath/Quantity/Unit.h"

#include <variant>

#include "Length.generated.h"

UENUM()
enum class ELengthUnit : uint8
{
    
    Centimeters UMETA(DisplayName="cm"),
    
    Meters UMETA(DisplayName="m"),
    
    Kilometers UMETA(DisplayName="km"),
    
};

template <class LengthUnit>
struct FLengthUnitTraits;


template <>
struct FLengthUnitTraits<SafeMath::Units::FCentimeters>
{
	constexpr static ELengthUnit LengthUnit = ELengthUnit::Centimeters;

	inline static const TCHAR* Suffix = TEXT("cm");
};

template <>
struct FLengthUnitTraits<SafeMath::Units::FMeters>
{
	constexpr static ELengthUnit LengthUnit = ELengthUnit::Meters;

	inline static const TCHAR* Suffix = TEXT("m");
};

template <>
struct FLengthUnitTraits<SafeMath::Units::FKilometers>
{
	constexpr static ELengthUnit LengthUnit = ELengthUnit::Kilometers;

	inline static const TCHAR* Suffix = TEXT("km");
};


using FLengthUnit = std::variant<

    SafeMath::Units::FCentimeters,

    SafeMath::Units::FMeters,

    SafeMath::Units::FKilometers

>;

constexpr FLengthUnit GetUnitVariant(const ELengthUnit LengthUnit)
{
	switch (LengthUnit)
	{

        case ELengthUnit::Centimeters:
			return FLengthUnit{ SafeMath::Units::FCentimeters{} };

        case ELengthUnit::Meters:
			return FLengthUnit{ SafeMath::Units::FMeters{} };

        case ELengthUnit::Kilometers:
			return FLengthUnit{ SafeMath::Units::FKilometers{} };

        default:
            return FLengthUnit{ SafeMath::Units::FCentimeters{} };
	}
}

USTRUCT()
struct FLength 
{
	GENERATED_BODY()

	using DefaultUnitType = SafeMath::Units::FCentimeters;

    using NumericType = double;

	using QuantityType = SafeMath::TQuantity<DefaultUnitType, NumericType>;

	using UnitEnumType = ELengthUnit;

	static FName GetValuePropertyName()
	{
		return GET_MEMBER_NAME_CHECKED(FLength, Value);
	}

	static FName GetSelectedUnitPropertyName()
	{
		return GET_MEMBER_NAME_CHECKED(FLength, SelectedUnit);
	}

	FLength() = default;

	template <class CompatibleUnitType>
	FLength(SafeMath::TQuantity<CompatibleUnitType, NumericType> Quantity)
		: Value(Quantity.template GetValue<DefaultUnitType>())
#if WITH_EDITORONLY_DATA
		, SelectedUnit(FLengthUnitTraits<CompatibleUnitType>::LengthUnit)
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
	ELengthUnit GetSelectedUnit() const
	{
		return SelectedUnit;
	}

	void SetSelectedUnit(ELengthUnit InSelectedUnit, ESetSelectedUnitStrategy SetSelectedUnitStrategy)
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

	FLength& operator*=(const NumericType Scalar)
	{
		Value *= Scalar;
		return *this;
	}

	FLength& operator/=(const NumericType Scalar)
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
	ELengthUnit SelectedUnit = FLengthUnitTraits<DefaultUnitType>::LengthUnit;
#endif
};

inline FLength operator*(const FLength Length, const double Scalar)
{
	return FLength{ Length } *= Scalar;
}

inline FLength operator*(const double Scalar, const FLength Length)
{
	return Length * Scalar;
}

inline FLength operator/(const FLength Length, const double Scalar)
{
	return FLength{ Length } /= Scalar;
}

inline FLength operator/(const double Scalar, const FLength Length)
{
	return Length / Scalar;
}

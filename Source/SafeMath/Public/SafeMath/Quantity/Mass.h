

// #TODO_dontcommit: add copyright everywhere (to file template also)
#pragma once

// #TODO_dontcommit: unreal types should be in a separate plugin I think. Others could just be in a third-party project

// #TODO_dontcommit: dimensions must be blueprint editable

#include "SetSelectedUnitStrategy.h"
#include "SafeMath/Quantity/Quantity.h"
#include "SafeMath/Quantity/Unit.h"

#include <variant>

#include "Mass.generated.h"

UENUM()
enum class EMassUnit : uint8
{
    
    Grams UMETA(DisplayName="g"),
    
    Kilograms UMETA(DisplayName="kg"),
    
};

template <class MassUnit>
struct FMassUnitTraits;


template <>
struct FMassUnitTraits<SafeMath::Units::FGrams>
{
	constexpr static EMassUnit MassUnit = EMassUnit::Grams;

	inline static const TCHAR* Suffix = TEXT("g");
};

template <>
struct FMassUnitTraits<SafeMath::Units::FKilograms>
{
	constexpr static EMassUnit MassUnit = EMassUnit::Kilograms;

	inline static const TCHAR* Suffix = TEXT("kg");
};


using FMassUnit = std::variant<

    SafeMath::Units::FGrams,

    SafeMath::Units::FKilograms

>;

constexpr FMassUnit GetUnitVariant(const EMassUnit MassUnit)
{
	switch (MassUnit)
	{

        case EMassUnit::Grams:
			return FMassUnit{ SafeMath::Units::FGrams{} };

        case EMassUnit::Kilograms:
			return FMassUnit{ SafeMath::Units::FKilograms{} };

        default:
            return FMassUnit{ SafeMath::Units::FKilograms{} };
	}
}

USTRUCT()
struct FMass 
{
	GENERATED_BODY()

	using DefaultUnitType = SafeMath::Units::FKilograms;

    using NumericType = double;

	using QuantityType = SafeMath::TQuantity<DefaultUnitType, NumericType>;

	using UnitEnumType = EMassUnit;

	static FName GetValuePropertyName()
	{
		return GET_MEMBER_NAME_CHECKED(FMass, Value);
	}

	static FName GetSelectedUnitPropertyName()
	{
		return GET_MEMBER_NAME_CHECKED(FMass, SelectedUnit);
	}

	FMass() = default;

	template <class CompatibleUnitType>
	FMass(SafeMath::TQuantity<CompatibleUnitType, NumericType> Quantity)
		: Value(Quantity.template GetValue<DefaultUnitType>())
#if WITH_EDITORONLY_DATA
		, SelectedUnit(FMassUnitTraits<CompatibleUnitType>::MassUnit)
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
	EMassUnit GetSelectedUnit() const
	{
		return SelectedUnit;
	}

	void SetSelectedUnit(EMassUnit InSelectedUnit, ESetSelectedUnitStrategy SetSelectedUnitStrategy)
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

	FMass& operator*=(const NumericType Scalar)
	{
		Value *= Scalar;
		return *this;
	}

	FMass& operator/=(const NumericType Scalar)
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
	EMassUnit SelectedUnit = FMassUnitTraits<DefaultUnitType>::MassUnit;
#endif
};

inline FMass operator*(const FMass Mass, const double Scalar)
{
	return FMass{ Mass } *= Scalar;
}

inline FMass operator*(const double Scalar, const FMass Mass)
{
	return Mass * Scalar;
}

inline FMass operator/(const FMass Mass, const double Scalar)
{
	return FMass{ Mass } /= Scalar;
}

inline FMass operator/(const double Scalar, const FMass Mass)
{
	return Mass / Scalar;
}

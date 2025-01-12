

// #TODO_dontcommit: add copyright everywhere (to file template also)
#pragma once

// #TODO_dontcommit: unreal types should be in a separate plugin I think. Others could just be in a third-party project

// #TODO_dontcommit: dimensions must be blueprint editable

#include "SetSelectedUnitStrategy.h"
#include "SafeMath/Quantity/Quantity.h"
#include "SafeMath/Quantity/Unit.h"

#include <variant>

#include "Angle.generated.h"

UENUM()
enum class EAngleUnit : uint8
{
    
    Degrees UMETA(DisplayName="deg"),
    
    Radians UMETA(DisplayName="rad"),
    
};

template <class AngleUnit>
struct FAngleUnitTraits;


template <>
struct FAngleUnitTraits<SafeMath::Units::FDegrees>
{
	constexpr static EAngleUnit AngleUnit = EAngleUnit::Degrees;

	inline static const TCHAR* Suffix = TEXT("deg");
};

template <>
struct FAngleUnitTraits<SafeMath::Units::FRadians>
{
	constexpr static EAngleUnit AngleUnit = EAngleUnit::Radians;

	inline static const TCHAR* Suffix = TEXT("rad");
};


using FAngleUnit = std::variant<

    SafeMath::Units::FDegrees,

    SafeMath::Units::FRadians

>;

constexpr FAngleUnit GetUnitVariant(const EAngleUnit AngleUnit)
{
	switch (AngleUnit)
	{

        case EAngleUnit::Degrees:
			return FAngleUnit{ SafeMath::Units::FDegrees{} };

        case EAngleUnit::Radians:
			return FAngleUnit{ SafeMath::Units::FRadians{} };

        default:
            return FAngleUnit{ SafeMath::Units::FDegrees{} };
	}
}

USTRUCT()
struct FAngle 
{
	GENERATED_BODY()

	using DefaultUnitType = SafeMath::Units::FDegrees;

    using NumericType = double;

	using QuantityType = SafeMath::TQuantity<DefaultUnitType, NumericType>;

	using UnitEnumType = EAngleUnit;

	static FName GetValuePropertyName()
	{
		return GET_MEMBER_NAME_CHECKED(FAngle, Value);
	}

	static FName GetSelectedUnitPropertyName()
	{
		return GET_MEMBER_NAME_CHECKED(FAngle, SelectedUnit);
	}

	FAngle() = default;

	template <class CompatibleUnitType>
	FAngle(SafeMath::TQuantity<CompatibleUnitType, NumericType> Quantity)
		: Value(Quantity.template GetValue<DefaultUnitType>())
#if WITH_EDITORONLY_DATA
		, SelectedUnit(FAngleUnitTraits<CompatibleUnitType>::AngleUnit)
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
	EAngleUnit GetSelectedUnit() const
	{
		return SelectedUnit;
	}

	void SetSelectedUnit(EAngleUnit InSelectedUnit, ESetSelectedUnitStrategy SetSelectedUnitStrategy)
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

	FAngle& operator*=(const NumericType Scalar)
	{
		Value *= Scalar;
		return *this;
	}

	FAngle& operator/=(const NumericType Scalar)
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
	EAngleUnit SelectedUnit = FAngleUnitTraits<DefaultUnitType>::AngleUnit;
#endif
};

inline FAngle operator*(const FAngle Angle, const double Scalar)
{
	return FAngle{ Angle } *= Scalar;
}

inline FAngle operator*(const double Scalar, const FAngle Angle)
{
	return Angle * Scalar;
}

inline FAngle operator/(const FAngle Angle, const double Scalar)
{
	return FAngle{ Angle } /= Scalar;
}

inline FAngle operator/(const double Scalar, const FAngle Angle)
{
	return Angle / Scalar;
}

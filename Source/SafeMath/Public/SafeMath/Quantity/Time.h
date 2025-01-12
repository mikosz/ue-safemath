

// #TODO_dontcommit: add copyright everywhere (to file template also)
#pragma once

// #TODO_dontcommit: unreal types should be in a separate plugin I think. Others could just be in a third-party project

// #TODO_dontcommit: dimensions must be blueprint editable

#include "SetSelectedUnitStrategy.h"
#include "SafeMath/Quantity/Quantity.h"
#include "SafeMath/Quantity/Unit.h"

#include <variant>

#include "Time.generated.h"

UENUM()
enum class ETimeUnit : uint8
{
    
    Seconds UMETA(DisplayName="s"),
    
    Hours UMETA(DisplayName="h"),
    
};

template <class TimeUnit>
struct FTimeUnitTraits;


template <>
struct FTimeUnitTraits<SafeMath::Units::FSeconds>
{
	constexpr static ETimeUnit TimeUnit = ETimeUnit::Seconds;

	inline static const TCHAR* Suffix = TEXT("s");
};

template <>
struct FTimeUnitTraits<SafeMath::Units::FHours>
{
	constexpr static ETimeUnit TimeUnit = ETimeUnit::Hours;

	inline static const TCHAR* Suffix = TEXT("h");
};


using FTimeUnit = std::variant<

    SafeMath::Units::FSeconds,

    SafeMath::Units::FHours

>;

constexpr FTimeUnit GetUnitVariant(const ETimeUnit TimeUnit)
{
	switch (TimeUnit)
	{

        case ETimeUnit::Seconds:
			return FTimeUnit{ SafeMath::Units::FSeconds{} };

        case ETimeUnit::Hours:
			return FTimeUnit{ SafeMath::Units::FHours{} };

        default:
            return FTimeUnit{ SafeMath::Units::FSeconds{} };
	}
}

USTRUCT()
struct FTime 
{
	GENERATED_BODY()

	using DefaultUnitType = SafeMath::Units::FSeconds;

    using NumericType = double;

	using QuantityType = SafeMath::TQuantity<DefaultUnitType, NumericType>;

	using UnitEnumType = ETimeUnit;

	static FName GetValuePropertyName()
	{
		return GET_MEMBER_NAME_CHECKED(FTime, Value);
	}

	static FName GetSelectedUnitPropertyName()
	{
		return GET_MEMBER_NAME_CHECKED(FTime, SelectedUnit);
	}

	FTime() = default;

	template <class CompatibleUnitType>
	FTime(SafeMath::TQuantity<CompatibleUnitType, NumericType> Quantity)
		: Value(Quantity.template GetValue<DefaultUnitType>())
#if WITH_EDITORONLY_DATA
		, SelectedUnit(FTimeUnitTraits<CompatibleUnitType>::TimeUnit)
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
	ETimeUnit GetSelectedUnit() const
	{
		return SelectedUnit;
	}

	void SetSelectedUnit(ETimeUnit InSelectedUnit, ESetSelectedUnitStrategy SetSelectedUnitStrategy)
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

	FTime& operator*=(const NumericType Scalar)
	{
		Value *= Scalar;
		return *this;
	}

	FTime& operator/=(const NumericType Scalar)
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
	ETimeUnit SelectedUnit = FTimeUnitTraits<DefaultUnitType>::TimeUnit;
#endif
};

inline FTime operator*(const FTime Time, const double Scalar)
{
	return FTime{ Time } *= Scalar;
}

inline FTime operator*(const double Scalar, const FTime Time)
{
	return Time * Scalar;
}

inline FTime operator/(const FTime Time, const double Scalar)
{
	return FTime{ Time } /= Scalar;
}

inline FTime operator/(const double Scalar, const FTime Time)
{
	return Time / Scalar;
}

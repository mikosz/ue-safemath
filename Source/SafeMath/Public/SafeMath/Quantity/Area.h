

// #TODO_dontcommit: add copyright everywhere (to file template also)
#pragma once

// #TODO_dontcommit: unreal types should be in a separate plugin I think. Others could just be in a third-party project

// #TODO_dontcommit: dimensions must be blueprint editable

#include "SetSelectedUnitStrategy.h"
#include "SafeMath/Quantity/Quantity.h"
#include "SafeMath/Quantity/Unit.h"

#include <variant>

#include "Area.generated.h"

UENUM()
enum class EAreaUnit : uint8
{
    
    CentimetersSquared UMETA(DisplayName="cm^2"),
    
    MetersSquared UMETA(DisplayName="m^2"),
    
};

template <class AreaUnit>
struct FAreaUnitTraits;


template <>
struct FAreaUnitTraits<SafeMath::Units::FCentimetersSquared>
{
	constexpr static EAreaUnit AreaUnit = EAreaUnit::CentimetersSquared;

	inline static const TCHAR* Suffix = TEXT("cm^2");
};

template <>
struct FAreaUnitTraits<SafeMath::Units::FMetersSquared>
{
	constexpr static EAreaUnit AreaUnit = EAreaUnit::MetersSquared;

	inline static const TCHAR* Suffix = TEXT("m^2");
};


using FAreaUnit = std::variant<

    SafeMath::Units::FCentimetersSquared,

    SafeMath::Units::FMetersSquared

>;

constexpr FAreaUnit GetUnitVariant(const EAreaUnit AreaUnit)
{
	switch (AreaUnit)
	{

        case EAreaUnit::CentimetersSquared:
			return FAreaUnit{ SafeMath::Units::FCentimetersSquared{} };

        case EAreaUnit::MetersSquared:
			return FAreaUnit{ SafeMath::Units::FMetersSquared{} };

        default:
            return FAreaUnit{ SafeMath::Units::FCentimetersSquared{} };
	}
}

USTRUCT()
struct FArea 
{
	GENERATED_BODY()

	using DefaultUnitType = SafeMath::Units::FCentimetersSquared;

    using NumericType = double;

	using QuantityType = SafeMath::TQuantity<DefaultUnitType, NumericType>;

	using UnitEnumType = EAreaUnit;

	static FName GetValuePropertyName()
	{
		return GET_MEMBER_NAME_CHECKED(FArea, Value);
	}

	static FName GetSelectedUnitPropertyName()
	{
		return GET_MEMBER_NAME_CHECKED(FArea, SelectedUnit);
	}

	FArea() = default;

	template <class CompatibleUnitType>
	FArea(SafeMath::TQuantity<CompatibleUnitType, NumericType> Quantity)
		: Value(Quantity.template GetValue<DefaultUnitType>())
#if WITH_EDITORONLY_DATA
		, SelectedUnit(FAreaUnitTraits<CompatibleUnitType>::AreaUnit)
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
	EAreaUnit GetSelectedUnit() const
	{
		return SelectedUnit;
	}

	void SetSelectedUnit(EAreaUnit InSelectedUnit, ESetSelectedUnitStrategy SetSelectedUnitStrategy)
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

	FArea& operator*=(const NumericType Scalar)
	{
		Value *= Scalar;
		return *this;
	}

	FArea& operator/=(const NumericType Scalar)
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
	EAreaUnit SelectedUnit = FAreaUnitTraits<DefaultUnitType>::AreaUnit;
#endif
};

inline FArea operator*(const FArea Area, const double Scalar)
{
	return FArea{ Area } *= Scalar;
}

inline FArea operator*(const double Scalar, const FArea Area)
{
	return Area * Scalar;
}

inline FArea operator/(const FArea Area, const double Scalar)
{
	return FArea{ Area } /= Scalar;
}

inline FArea operator/(const double Scalar, const FArea Area)
{
	return Area / Scalar;
}

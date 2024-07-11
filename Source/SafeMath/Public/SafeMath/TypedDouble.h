// #TODO_dontcommit: add copyright everywhere (to file template also)
#pragma once

// #TODO_dontcommit: why the filename?
// #TODO_dontcommit: unreal types should be in a separate plugin I think. Others could just be in a third-party project

#include "Quantity.h"
#include "Unit.h"

#include <variant>

#include "TypedDouble.generated.h"

UENUM()
enum class ELengthUnit : uint8
{

	Centimeters,

	Meters,

	Kilometers,

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

constexpr FLengthUnit GetLengthUnit(const ELengthUnit LengthUnit)
{
	switch (LengthUnit)
	{
		case ELengthUnit::Centimeters:
			return FLengthUnit{SafeMath::Units::FCentimeters{}};

		case ELengthUnit::Meters:
			return FLengthUnit{SafeMath::Units::FMeters{}};

		case ELengthUnit::Kilometers:
			return FLengthUnit{SafeMath::Units::FKilometers{}};
	}
}

USTRUCT()

struct FLength
{
	GENERATED_BODY()

	using DefaultUnitType = SafeMath::Units::FCentimeters;

	using QuantityType = SafeMath::TQuantity<DefaultUnitType, double>;

	FLength() = default;

	FLength(QuantityType InValue) : Value(InValue.GetValue<DefaultUnitType>())
	{
	}

	operator QuantityType() const
	{
		return GetValue();
	}

	QuantityType GetValue() const
	{
		return QuantityType{DefaultUnitType{}, Value};
	}

private:
	// Always internally stored in default dimension unit (conversion rate == 1)
	UPROPERTY(EditAnywhere)
	double Value = 0.;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	ELengthUnit SelectedUnit = FLengthUnitTraits<DefaultUnitType>::LengthUnit;
#endif
};

UENUM()
enum class EMassUnit : uint8
{

	Grams,

	Kilograms,

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

constexpr FMassUnit GetMassUnit(const EMassUnit MassUnit)
{
	switch (MassUnit)
	{
		case EMassUnit::Grams:
			return FMassUnit{SafeMath::Units::FGrams{}};

		case EMassUnit::Kilograms:
			return FMassUnit{SafeMath::Units::FKilograms{}};
	}
}

USTRUCT()

struct FMass
{
	GENERATED_BODY()

	using DefaultUnitType = SafeMath::Units::FKilograms;

	using QuantityType = SafeMath::TQuantity<DefaultUnitType, double>;

	FMass() = default;

	FMass(QuantityType InValue) : Value(InValue.GetValue<DefaultUnitType>())
	{
	}

	operator QuantityType() const
	{
		return QuantityType{DefaultUnitType{}, Value};
	}

	template <class UnitType>
	double GetValue() const
	{
		return QuantityType{DefaultUnitType{}, Value}.GetValue<UnitType>();
	}

	FMass& operator*=(double Scalar)
	{
		Value *= Scalar;
		return *this;
	}

	FMass& operator/=(double Scalar)
	{
		Value /= Scalar;
		return *this;
	}

private:
	// Always internally stored in default dimension unit (conversion rate == 1)
	UPROPERTY(EditAnywhere)
	double Value = 0.;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	EMassUnit SelectedUnit = FMassUnitTraits<DefaultUnitType>::MassUnit;
#endif
};

inline FMass operator*(const FMass Mass, const double Scalar)
{
	return FMass{Mass} *= Scalar;
}

inline FMass operator*(const double Scalar, const FMass Mass)
{
	return Mass * Scalar;
}

inline FMass operator/(const FMass Mass, const double Scalar)
{
	return FMass{Mass} /= Scalar;
}

inline FMass operator/(const double Scalar, const FMass Mass)
{
	return Mass / Scalar;
}

// #TODO_dontcommit: add copyright everywhere (to file template also)
#pragma once

// #TODO_dontcommit: why the filename?
// #TODO_dontcommit: unreal types should be in a separate plugin I think. Others could just be in a third-party project

// #TODO_dontcommit: dimensions must be blueprint editable

#include "Quantity.h"
#include "Unit.h"

#include <variant>

#include "TypedDouble.generated.h"

// #TODO_dontcommit should be in separate include file
UENUM()
enum class ESetSelectedUnitStrategy : uint8
{
	// Keep the numerical value the same while changing unit. E.g. for mass this will convert 3 grams to 3 kilograms
	KeepNumericalValue,
	// Keep the represented quantity the same. E.g. for mass this will convert 3 grams to 0.003 kilograms
	KeepQuantityValue,
};





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



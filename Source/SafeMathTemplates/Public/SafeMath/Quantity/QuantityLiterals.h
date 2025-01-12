#pragma once

#include "Quantity.h"

namespace SafeMath::QuantityLiterals
{




inline TQuantity<Units::FDegrees, double> operator""_deg(long double Value)
{
	return TQuantity{Units::FDegrees{}, static_cast<double>(Value)};
}

inline TQuantity<Units::FDegrees, double> operator""_deg(unsigned long long Value)
{
	return TQuantity{Units::FDegrees{}, static_cast<double>(Value)};
}


inline TQuantity<Units::FRadians, double> operator""_rad(long double Value)
{
	return TQuantity{Units::FRadians{}, static_cast<double>(Value)};
}

inline TQuantity<Units::FRadians, double> operator""_rad(unsigned long long Value)
{
	return TQuantity{Units::FRadians{}, static_cast<double>(Value)};
}




inline TQuantity<Units::FCentimeters, double> operator""_cm(long double Value)
{
	return TQuantity{Units::FCentimeters{}, static_cast<double>(Value)};
}

inline TQuantity<Units::FCentimeters, double> operator""_cm(unsigned long long Value)
{
	return TQuantity{Units::FCentimeters{}, static_cast<double>(Value)};
}


inline TQuantity<Units::FMeters, double> operator""_m(long double Value)
{
	return TQuantity{Units::FMeters{}, static_cast<double>(Value)};
}

inline TQuantity<Units::FMeters, double> operator""_m(unsigned long long Value)
{
	return TQuantity{Units::FMeters{}, static_cast<double>(Value)};
}


inline TQuantity<Units::FKilometers, double> operator""_km(long double Value)
{
	return TQuantity{Units::FKilometers{}, static_cast<double>(Value)};
}

inline TQuantity<Units::FKilometers, double> operator""_km(unsigned long long Value)
{
	return TQuantity{Units::FKilometers{}, static_cast<double>(Value)};
}




inline TQuantity<Units::FGrams, double> operator""_g(long double Value)
{
	return TQuantity{Units::FGrams{}, static_cast<double>(Value)};
}

inline TQuantity<Units::FGrams, double> operator""_g(unsigned long long Value)
{
	return TQuantity{Units::FGrams{}, static_cast<double>(Value)};
}


inline TQuantity<Units::FKilograms, double> operator""_kg(long double Value)
{
	return TQuantity{Units::FKilograms{}, static_cast<double>(Value)};
}

inline TQuantity<Units::FKilograms, double> operator""_kg(unsigned long long Value)
{
	return TQuantity{Units::FKilograms{}, static_cast<double>(Value)};
}




inline TQuantity<Units::FCentimetersSquared, double> operator""_cm2(long double Value)
{
	return TQuantity{Units::FCentimetersSquared{}, static_cast<double>(Value)};
}

inline TQuantity<Units::FCentimetersSquared, double> operator""_cm2(unsigned long long Value)
{
	return TQuantity{Units::FCentimetersSquared{}, static_cast<double>(Value)};
}


inline TQuantity<Units::FMetersSquared, double> operator""_m2(long double Value)
{
	return TQuantity{Units::FMetersSquared{}, static_cast<double>(Value)};
}

inline TQuantity<Units::FMetersSquared, double> operator""_m2(unsigned long long Value)
{
	return TQuantity{Units::FMetersSquared{}, static_cast<double>(Value)};
}




inline TQuantity<Units::FSeconds, double> operator""_s(long double Value)
{
	return TQuantity{Units::FSeconds{}, static_cast<double>(Value)};
}

inline TQuantity<Units::FSeconds, double> operator""_s(unsigned long long Value)
{
	return TQuantity{Units::FSeconds{}, static_cast<double>(Value)};
}


inline TQuantity<Units::FHours, double> operator""_h(long double Value)
{
	return TQuantity{Units::FHours{}, static_cast<double>(Value)};
}

inline TQuantity<Units::FHours, double> operator""_h(unsigned long long Value)
{
	return TQuantity{Units::FHours{}, static_cast<double>(Value)};
}




inline TQuantity<Units::FMetersPerSecond, double> operator""_mps(long double Value)
{
	return TQuantity{Units::FMetersPerSecond{}, static_cast<double>(Value)};
}

inline TQuantity<Units::FMetersPerSecond, double> operator""_mps(unsigned long long Value)
{
	return TQuantity{Units::FMetersPerSecond{}, static_cast<double>(Value)};
}


inline TQuantity<Units::FKilometersPerHour, double> operator""_kmph(long double Value)
{
	return TQuantity{Units::FKilometersPerHour{}, static_cast<double>(Value)};
}

inline TQuantity<Units::FKilometersPerHour, double> operator""_kmph(unsigned long long Value)
{
	return TQuantity{Units::FKilometersPerHour{}, static_cast<double>(Value)};
}




inline TQuantity<Units::FMetersPerSecondPerSecond, double> operator""_mps2(long double Value)
{
	return TQuantity{Units::FMetersPerSecondPerSecond{}, static_cast<double>(Value)};
}

inline TQuantity<Units::FMetersPerSecondPerSecond, double> operator""_mps2(unsigned long long Value)
{
	return TQuantity{Units::FMetersPerSecondPerSecond{}, static_cast<double>(Value)};
}



}  // namespace SafeMath::QuantityLiterals

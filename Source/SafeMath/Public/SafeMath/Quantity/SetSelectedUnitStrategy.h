#pragma once

UENUM()
enum class ESetSelectedUnitStrategy : uint8
{
	// Keep the numerical value the same while changing unit. E.g. for mass this will convert 3 grams to 3 kilograms
	KeepNumericalValue,
	// Keep the represented quantity the same. E.g. for mass this will convert 3 grams to 0.003 kilograms
	KeepQuantityValue,
};

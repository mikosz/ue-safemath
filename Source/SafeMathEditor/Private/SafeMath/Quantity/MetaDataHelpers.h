#pragma once

#include "CoreMinimal.h"

#include "SafeMathEditorLog.h"

namespace SafeMath::Editor::Private
{

template <class NumericType>
struct FLimitValue
{
	NumericType Value;
	FString Unit;
};

template <class NumericType>
TOptional<FLimitValue<NumericType>> GetLimitValue(
	const FString& Value,
	const FString& Units,
	const FString& ForceUnits,
	const FText& PropertyDisplayName,
	const FString& MetaDataName)
{
	if (Value.IsEmpty())
	{
		return NullOpt;
	}

	if (Units.IsEmpty() && ForceUnits.IsEmpty())
	{
		UE_LOG(
			LogSafeMathEditor,
			Warning,
			TEXT("Property %s has %s set but neither Units nor ForceUnits are set. Limit will be ignored."),
			*PropertyDisplayName.ToString(),
			*MetaDataName);
	}

	return FLimitValue{Parse<NumericType>(Value), ForceUnits.IsEmpty() ? Units : ForceUnits};
}

}  // namespace SafeMath::Editor::Private

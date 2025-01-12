#pragma once

#include "CoreMinimal.h"

#include "MathUtils.h"

namespace SafeMath
{

// #TODO_dontcommit put in other file (probably separate utility plugin as another submodule like errant utilities)

/// If all property values are the same, return the common value, otherwise NullOpt
template <class T>
TOptional<T> GetCommonPropertyValue(const IPropertyHandle& PropertyHandle)
{
	TOptional<T> CommonValue;

	const auto RawDataCallback = [&CommonValue](const void* RawData, const int32 DataIndex, const int32 NumDatas)
	{
		const T* NextRawData = static_cast<const T*>(RawData);
		if (!CommonValue.IsSet())
		{
			CommonValue = *NextRawData;
		}
		else if (!IsNearlyEqual(CommonValue.GetValue(), *NextRawData))
		{
			CommonValue.Reset();
			return false;
		}
		return true;
	};

	PropertyHandle.EnumerateConstRawData(RawDataCallback);

	return CommonValue;
}

template <class T>
void SetPropertyValue(IPropertyHandle& PropertyHandle, const T& Value)
{
	const auto RawDataCallback = [&Value](void* RawData, const int32 DataIndex, const int32 NumDatas)
	{
		*static_cast<T*>(RawData) = Value;
		return true;
	};

	PropertyHandle.NotifyPreChange();
	PropertyHandle.EnumerateRawData(RawDataCallback);
	PropertyHandle.NotifyPostChange(EPropertyChangeType::ValueSet);
}

template <class T, class F>
void ForEachPropertyValue(IPropertyHandle& PropertyHandle, const F& Function)
{
	const auto RawDataCallback = [&Function](void* RawData, const int32 DataIndex, const int32 NumDatas) -> bool
	{ return Invoke(Function, *static_cast<T*>(RawData)); };

	PropertyHandle.NotifyPreChange();
	PropertyHandle.EnumerateRawData(RawDataCallback);
	PropertyHandle.NotifyPostChange(EPropertyChangeType::ValueSet);
}

}  // namespace SafeMath

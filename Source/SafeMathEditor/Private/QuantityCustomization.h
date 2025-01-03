// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DetailWidgetRow.h"
#include "QuantityWidget.h"

namespace SafeMath::Editor
{

template <class QuantityStructType>
class FQuantityCustomization final : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	virtual void CustomizeChildren(
		TSharedRef<IPropertyHandle> PropertyHandle,
		IDetailChildrenBuilder& ChildBuilder,
		IPropertyTypeCustomizationUtils& CustomizationUtils) override;

	virtual void CustomizeHeader(
		TSharedRef<IPropertyHandle> PropertyHandle,
		FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& CustomizationUtils) override;
};

template <class QuantityStructType>
TSharedRef<IPropertyTypeCustomization> FQuantityCustomization<QuantityStructType>::MakeInstance()
{
	return MakeShared<FQuantityCustomization>();
}

template <class QuantityStructType>
void FQuantityCustomization<QuantityStructType>::CustomizeChildren(
	TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder,
	IPropertyTypeCustomizationUtils& CustomizationUtils)
{
}

template <class QuantityStructType>
void FQuantityCustomization<QuantityStructType>::CustomizeHeader(
	TSharedRef<IPropertyHandle> PropertyHandle,
	FDetailWidgetRow& HeaderRow,
	IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	const UEnum* const UnitEnum = StaticEnum<typename QuantityStructType::UnitEnumType>();
	const TSharedPtr<IPropertyHandle> ValuePropertyHandle =
		PropertyHandle->GetChildHandle(QuantityStructType::GetValuePropertyName());
	const TSharedPtr<IPropertyHandle> SelectedUnitPropertyHandle =
		PropertyHandle->GetChildHandle(QuantityStructType::GetSelectedUnitPropertyName());
	if (!IsValid(UnitEnum) || !ValuePropertyHandle.IsValid() || !SelectedUnitPropertyHandle.IsValid())
	{
		return;
	}

	// clang-format off
	HeaderRow
		.NameContent()
		[
			PropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SNew(SQuantityWidget<QuantityStructType>, PropertyHandle)
		];
	// clang-format on
}

}  // namespace SafeMath::Editor

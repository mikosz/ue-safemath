#pragma once

#include "PropertyUtils.h"
#include "SafeMath/TypedDouble.h"
#include "Widgets/Input/STextComboBox.h"

#define LOCTEXT_NAMESPACE "SafeMath_QuantityUnitWidget"

namespace SafeMath::Editor
{

template <class QuantityStructType>
class SQuantityUnitWidget : public STextComboBox
{
public:
	SLATE_BEGIN_ARGS(SQuantityUnitWidget) : _Font{FAppStyle::GetFontStyle(TEXT("PropertyWindow.NormalFont"))}
	{
	}

	SLATE_ATTRIBUTE(FSlateFontInfo, Font)

	SLATE_END_ARGS()

	SQuantityUnitWidget();

	void Construct(const FArguments& Args, const TSharedRef<IPropertyHandle>& InPropertyHandle);

private:
	using UnitEnumType = typename QuantityStructType::UnitEnumType;

	TSharedPtr<IPropertyHandle> PropertyHandle;

	TArray<TSharedPtr<FString>> UnitTypes;

	bool DoIsEnabled() const;

	void OnSelectionChanged(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo) const;
};

template <class QuantityStructType>
SQuantityUnitWidget<QuantityStructType>::SQuantityUnitWidget()
{
	const UEnum* const UnitEnum = StaticEnum<UnitEnumType>();
	if (!IsValid(UnitEnum))
	{
		return;
	}

	const int32 NumUnits = UnitEnum->NumEnums() - (UnitEnum->ContainsExistingMax() ? 1 : 0);
	UnitTypes.Reserve(NumUnits);

	for (int32 UnitIdx = 0; UnitIdx < NumUnits; ++UnitIdx)
	{
		UnitTypes.Emplace(MakeShared<FString>(UnitEnum->GetDisplayNameTextByIndex(UnitIdx).ToString()));
	}
}

template <class QuantityStructType>
void SQuantityUnitWidget<QuantityStructType>::Construct(
	const FArguments& Args, const TSharedRef<IPropertyHandle>& InPropertyHandle)
{
	const UEnum* const UnitEnum = StaticEnum<UnitEnumType>();
	PropertyHandle = InPropertyHandle;
	if (!IsValid(UnitEnum) || !PropertyHandle.IsValid())
	{
		return;
	}

	const TSharedPtr<IPropertyHandle> SelectedUnitPropertyHandle =
		InPropertyHandle->GetChildHandle(QuantityStructType::GetSelectedUnitPropertyName());
	if (!SelectedUnitPropertyHandle.IsValid())
	{
		return;
	}

	const TOptional<UnitEnumType> InitiallySelectedUnit =
		GetCommonPropertyValue<UnitEnumType>(*SelectedUnitPropertyHandle);
	TSharedPtr<FString> InitiallySelectedItem =
		InitiallySelectedUnit.IsSet()
			? UnitTypes[UnitEnum->GetIndexByValue(static_cast<int64>(InitiallySelectedUnit.GetValue()))]
			: nullptr;

	STextComboBox::Construct(STextComboBox::FArguments{}
								 .Font(Args._Font)
								 .OptionsSource(&UnitTypes)
								 .InitiallySelectedItem(InitiallySelectedItem)
								 .IsEnabled_Raw(this, &SQuantityUnitWidget::DoIsEnabled)
								 .OnSelectionChanged_Raw(this, &SQuantityUnitWidget::OnSelectionChanged));
}

template <class QuantityStructType>
bool SQuantityUnitWidget<QuantityStructType>::DoIsEnabled() const
{
	const TSharedPtr<IPropertyHandle> ValuePropertyHandle =
		PropertyHandle->GetChildHandle(QuantityStructType::GetValuePropertyName());
	const TSharedPtr<IPropertyHandle> SelectedUnitPropertyHandle =
		PropertyHandle->GetChildHandle(QuantityStructType::GetSelectedUnitPropertyName());
	if (!ValuePropertyHandle.IsValid() || !SelectedUnitPropertyHandle.IsValid())
	{
		return false;
	}

	const TOptional<UnitEnumType> CommonSelectedUnit =
		GetCommonPropertyValue<UnitEnumType>(*SelectedUnitPropertyHandle);
	const TOptional<double> CommonValue = GetCommonPropertyValue<double>(*ValuePropertyHandle);
	return CommonSelectedUnit.IsSet() && CommonValue.IsSet();
}

template <class QuantityStructType>
void SQuantityUnitWidget<QuantityStructType>::OnSelectionChanged(
	TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo) const
{
	const UEnum* const UnitEnum = StaticEnum<UnitEnumType>();

	if (!IsValid(UnitEnum) || !PropertyHandle.IsValid())
	{
		return;
	}

	const int32 UnitIdx = UnitTypes.Find(Item);

	if (UnitIdx == INDEX_NONE)
	{
		return;
	}

	{
		FScopedTransaction Transaction{
			FText::Format(LOCTEXT("SetUnit", "Edit {0}"), PropertyHandle->GetPropertyDisplayName())};

		const UnitEnumType SelectedUnit = static_cast<UnitEnumType>(UnitEnum->GetValueByIndex(UnitIdx));

		ForEachPropertyValue<QuantityStructType>(
			*PropertyHandle,
			[SelectedUnit](QuantityStructType& Quantity)
			{
				Quantity.SetSelectedUnit(SelectedUnit, ESetSelectedUnitStrategy::KeepNumericalValue);
				return true;
			});
	}
}

}  // namespace SafeMath::Editor

#undef LOCTEXT_NAMESPACE

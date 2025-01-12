#pragma once

#include "PropertyUtils.h"
#include "Widgets/Input/SNumericEntryBox.h"

#include <variant>

#define LOCTEXT_NAMESPACE "SafeMath_QuantityValueWidget"

namespace SafeMath::Editor
{

template <class QuantityStructType>
class SQuantityValueWidget : public SNumericEntryBox<typename QuantityStructType::NumericType>
{
public:
	SLATE_BEGIN_ARGS(SQuantityValueWidget) : _Font{FAppStyle::GetFontStyle(TEXT("PropertyWindow.NormalFont"))}
	{
	}

	SLATE_ATTRIBUTE(FSlateFontInfo, Font)

	SLATE_END_ARGS()

	using NumericType = typename QuantityStructType::NumericType;

	void Construct(const FArguments& Args, const TSharedRef<IPropertyHandle>& InPropertyHandle);

private:
	using UnitEnumType = typename QuantityStructType::UnitEnumType;

	using DefaultUnitType = typename QuantityStructType::DefaultUnitType;

	TSharedPtr<IPropertyHandle> PropertyHandle;

	TOptional<NumericType> DoGetValue() const;

	void DoOnValueCommitted(const NumericType NewValue, ETextCommit::Type CommitType) const;

	bool DoIsEnabled() const;
};

template <class NumericType>
void SQuantityValueWidget<NumericType>::Construct(
	const FArguments& Args, const TSharedRef<IPropertyHandle>& InPropertyHandle)
{
	PropertyHandle = InPropertyHandle;

	SNumericEntryBox<NumericType>::Construct(typename SNumericEntryBox<NumericType>::FArguments{}
												 .Font(Args._Font)
												 .Value_Raw(this, &SQuantityValueWidget::DoGetValue)
												 .OnValueCommitted_Raw(this, &SQuantityValueWidget::DoOnValueCommitted)
												 .IsEnabled_Raw(this, &SQuantityValueWidget::DoIsEnabled));
	// #TODO_dontcommit:
	// also should handle metadata similar as in SPropertyEditorNumeric.h:335
}

template <class QuantityStructType>
void SQuantityValueWidget<QuantityStructType>::DoOnValueCommitted(
	const NumericType NewValue, ETextCommit::Type CommitType) const
{
	const TSharedPtr<IPropertyHandle> ValuePropertyHandle =
		PropertyHandle->GetChildHandle(QuantityStructType::GetValuePropertyName());
	const TSharedPtr<IPropertyHandle> SelectedUnitPropertyHandle =
		PropertyHandle->GetChildHandle(QuantityStructType::GetSelectedUnitPropertyName());

	if (!PropertyHandle.IsValid() || !SelectedUnitPropertyHandle.IsValid() || !ValuePropertyHandle.IsValid())
	{
		return;
	}

	const TOptional<UnitEnumType> CommonSelectedUnit =
		GetCommonPropertyValue<UnitEnumType>(*SelectedUnitPropertyHandle);
	if (!CommonSelectedUnit.IsSet())
	{
		return;
	}

	const auto SelectedUnitVariant = GetUnitVariant(CommonSelectedUnit.GetValue());
	const NumericType ConversionRate = std::visit(
		[](auto SelectedUnit)
		{ return GetConversionRateBetween(SelectedUnit, DefaultUnitType{}); },
		SelectedUnitVariant);
	const NumericType NewPropertyValue = ConversionRate * NewValue;

	const TOptional<NumericType> CommonValue = GetCommonPropertyValue<NumericType>(*ValuePropertyHandle);
	if (CommonValue.IsSet() && IsNearlyEqual(CommonValue.GetValue(), NewPropertyValue))
	{
		return;
	}

	{
		FScopedTransaction Transaction{
			FText::Format(LOCTEXT("SetValue", "Edit {0}"), PropertyHandle->GetPropertyDisplayName())};
		SetPropertyValue(*ValuePropertyHandle, NewPropertyValue);
	}
}

template <class QuantityStructType>
bool SQuantityValueWidget<QuantityStructType>::DoIsEnabled() const
{
	const TSharedPtr<IPropertyHandle> SelectedUnitPropertyHandle =
		PropertyHandle->GetChildHandle(QuantityStructType::GetSelectedUnitPropertyName());
	if (!SelectedUnitPropertyHandle.IsValid())
	{
		return false;
	}

	const TOptional<UnitEnumType> CommonSelectedUnit =
		GetCommonPropertyValue<UnitEnumType>(*SelectedUnitPropertyHandle);

	return CommonSelectedUnit.IsSet();
}

template <class QuantityStructType>
TOptional<typename SQuantityValueWidget<QuantityStructType>::NumericType> SQuantityValueWidget<
	QuantityStructType>::DoGetValue() const
{
	const TSharedPtr<IPropertyHandle> ValuePropertyHandle =
		PropertyHandle->GetChildHandle(QuantityStructType::GetValuePropertyName());
	const TSharedPtr<IPropertyHandle> SelectedUnitPropertyHandle =
		PropertyHandle->GetChildHandle(QuantityStructType::GetSelectedUnitPropertyName());
	if (!ValuePropertyHandle.IsValid() || !SelectedUnitPropertyHandle.IsValid())
	{
		return false;
	}

	const TOptional<NumericType> CommonValue = GetCommonPropertyValue<NumericType>(*ValuePropertyHandle);
	if (!CommonValue.IsSet())
	{
		return NullOpt;
	}

	const TOptional<UnitEnumType> CommonSelectedUnit =
		GetCommonPropertyValue<UnitEnumType>(*SelectedUnitPropertyHandle);
	if (!CommonSelectedUnit.IsSet())
	{
		return NullOpt;
	}

	const auto SelectedUnitVariant = GetUnitVariant(CommonSelectedUnit.GetValue());
	const NumericType ConversionRate = std::visit(
		[](auto SelectedUnit)
		{ return GetConversionRateBetween(DefaultUnitType{}, SelectedUnit); },
		SelectedUnitVariant);

	return ConversionRate * CommonValue.GetValue();
}

}  // namespace SafeMath::Editor

#undef LOCTEXT_NAMESPACE

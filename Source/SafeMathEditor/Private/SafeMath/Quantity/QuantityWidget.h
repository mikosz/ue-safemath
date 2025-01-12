#pragma once
#include "QuantityUnitWidget.h"
#include "QuantityValueWidget.h"

namespace SafeMath::Editor
{

template <class QuantityStructType>
class SQuantityWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SQuantityWidget) : _Font(FAppStyle::GetFontStyle(TEXT("PropertyWindow.NormalFont")))
	{
	}

	SLATE_ATTRIBUTE(FSlateFontInfo, Font)

	SLATE_END_ARGS()

	void Construct(const FArguments& Args, TSharedRef<IPropertyHandle> PropertyHandle);
};

template <class QuantityStructType>
void SQuantityWidget<QuantityStructType>::Construct(const FArguments& Args, TSharedRef<IPropertyHandle> PropertyHandle)
{
	// clang-format off
	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		[
			SNew(SQuantityValueWidget<QuantityStructType>, PropertyHandle)
				.Font(Args._Font)
		]
		+ SHorizontalBox::Slot()
		[
			SNew(SQuantityUnitWidget<QuantityStructType>, PropertyHandle)
				.Font(Args._Font)
		]
	];
	// clang-format on
}

}  // namespace SafeMath::Editor

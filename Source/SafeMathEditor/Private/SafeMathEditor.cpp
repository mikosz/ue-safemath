#include "SafeMathEditor.h"

#include "QuantityCustomization.h"
#include "RegisterCustomPropertyTypeLayouts.h"

#define LOCTEXT_NAMESPACE "FSafeMathEditorModule"

void FSafeMathEditorModule::StartupModule()
{
	using namespace SafeMath::Editor;

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	RegisterCustomPropertyTypeLayouts(PropertyModule);
}

void FSafeMathEditorModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSafeMathEditorModule, SafeMathEditor)

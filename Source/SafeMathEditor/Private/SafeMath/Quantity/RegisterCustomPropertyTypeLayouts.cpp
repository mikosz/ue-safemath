#include "RegisterCustomPropertyTypeLayouts.h"

#include "QuantityCustomization.h"



#include "SafeMath/Quantity/Angle.h"


#include "SafeMath/Quantity/Length.h"


#include "SafeMath/Quantity/Mass.h"


#include "SafeMath/Quantity/Area.h"


#include "SafeMath/Quantity/Time.h"


#include "SafeMath/Quantity/Speed.h"


#include "SafeMath/Quantity/Acceleration.h"


namespace SafeMath::Editor
{

void RegisterCustomPropertyTypeLayouts(FPropertyEditorModule& PropertyModule)
{

    
    PropertyModule.RegisterCustomPropertyTypeLayout(
    	FAngle::StaticStruct()->GetFName(),
    	FOnGetPropertyTypeCustomizationInstance::CreateStatic(FQuantityCustomization<FAngle>::MakeInstance));

    
    PropertyModule.RegisterCustomPropertyTypeLayout(
    	FLength::StaticStruct()->GetFName(),
    	FOnGetPropertyTypeCustomizationInstance::CreateStatic(FQuantityCustomization<FLength>::MakeInstance));

    
    PropertyModule.RegisterCustomPropertyTypeLayout(
    	FMass::StaticStruct()->GetFName(),
    	FOnGetPropertyTypeCustomizationInstance::CreateStatic(FQuantityCustomization<FMass>::MakeInstance));

    
    PropertyModule.RegisterCustomPropertyTypeLayout(
    	FArea::StaticStruct()->GetFName(),
    	FOnGetPropertyTypeCustomizationInstance::CreateStatic(FQuantityCustomization<FArea>::MakeInstance));

    
    PropertyModule.RegisterCustomPropertyTypeLayout(
    	FTime::StaticStruct()->GetFName(),
    	FOnGetPropertyTypeCustomizationInstance::CreateStatic(FQuantityCustomization<FTime>::MakeInstance));

    
    PropertyModule.RegisterCustomPropertyTypeLayout(
    	FSpeed::StaticStruct()->GetFName(),
    	FOnGetPropertyTypeCustomizationInstance::CreateStatic(FQuantityCustomization<FSpeed>::MakeInstance));

    
    PropertyModule.RegisterCustomPropertyTypeLayout(
    	FAcceleration::StaticStruct()->GetFName(),
    	FOnGetPropertyTypeCustomizationInstance::CreateStatic(FQuantityCustomization<FAcceleration>::MakeInstance));

}

}  // namespace SafeMath::Editor

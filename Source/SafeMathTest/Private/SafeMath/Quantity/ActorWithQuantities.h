#pragma once

#include <CoreMinimal.h>

#include "SafeMath/Quantity/Acceleration.h"
#include "SafeMath/Quantity/Area.h"
#include "SafeMath/Quantity/Length.h"
#include "SafeMath/Quantity/Mass.h"
#include "SafeMath/Quantity/QuantityLiterals.h"

#include "ActorWithQuantities.generated.h"

using namespace SafeMath::QuantityLiterals;

UCLASS() class AActorWithQuantities : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FLength Length = 20_cm;

	UPROPERTY(EditAnywhere)
	FMass Mass = 0.3_kg;

	UPROPERTY(EditAnywhere)
	FAcceleration Acceleration = 9.81_mps2;

	UPROPERTY(EditAnywhere)
	FArea Area = 20_cm2;
};

#pragma once

#include <CoreMinimal.h>

#include "SafeMath/TypedDouble.h"

#include "ActorWithQuantities.generated.h"

UCLASS()
class AActorWithQuantities : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FLength Length;

	UPROPERTY(EditAnywhere)
	FMass Mass;

	UPROPERTY(EditAnywhere)
	FAcceleration Acceleration;

	UPROPERTY(EditAnywhere)
	FArea Area;
};

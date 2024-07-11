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
};
 
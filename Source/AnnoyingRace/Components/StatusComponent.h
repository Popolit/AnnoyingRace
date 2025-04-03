#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"

/*
 * Status for Character
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANNOYINGRACE_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatusComponent();

private:
};

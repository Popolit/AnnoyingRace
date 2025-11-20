#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TargetingMechanism.generated.h"

/**
 * Targeting Mechanism
 */
UCLASS(Abstract, HideDropDown, EditInlineNew)
class ANNOYINGRACE_API UTargetingMechanism : public UObject
{
	GENERATED_BODY()

public:
	virtual void SetTargets(ACharacter* _Character) PURE_VIRTUAL(...);
	
	virtual void GetTargets(TArray<AActor*>& _OutTargets);

protected:
	TArray<AActor*> TargetActors_;
};

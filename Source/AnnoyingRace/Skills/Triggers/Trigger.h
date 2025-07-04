#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Trigger.generated.h"

/**
 * Skill Trigger
 */
UCLASS(Abstract, HideDropDown)
class ANNOYINGRACE_API UTrigger : public UObject
{
	GENERATED_BODY()

public:
	virtual void Initialize(ACharacter* _Character, class USkill* _OwnerSkill) PURE_VIRTUAL(...);
};

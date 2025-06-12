#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "Skill_NonTarget.generated.h"

/**
 * Non-Target Skills
 */
UCLASS(Abstract, HideDropdown)
class ANNOYINGRACE_API USkill_NonTarget : public USkill
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
		TObjectPtr<UShapeComponent> Collision_;

	UPROPERTY(EditDefaultsOnly)
		FTransform Transform_;
};

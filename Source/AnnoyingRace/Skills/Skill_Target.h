#pragma once

#include "CoreMinimal.h"
#include "Skills/Skill.h"
#include "Skill_Target.generated.h"

/**
 * Skill (Targeting)
 */
UCLASS(Abstract, HideDropdown)
class ANNOYINGRACE_API USkill_Target : public USkill
{
	GENERATED_BODY()

public:
	void SetTargets(ACharacter* _Character);

	void GetTargets(TArray<AActor*>& _OutTargets) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Instanced, Category = Skill)
		TObjectPtr<class UTargetingMechanism> TargetingMechanism_;
};

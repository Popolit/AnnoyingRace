#pragma once

#include "CoreMinimal.h"
#include "Skill.generated.h"

/**
 * Skills UObject
 */
UCLASS(Abstract, NotBlueprintable, HideDropdown)
class ANNOYINGRACE_API USkill : public UObject
{
	GENERATED_BODY()

public:
	USkill();

	//NewObject<>(SkillTrigger)를 해 줄 것.
	//자식의 Initialize가 끝난 후 Super()를 호출할 것. 
	virtual void Initialize(ACharacter* _Character);

	uint8 GetDamage() const;

protected:
	virtual void TryTriggerSkill(ACharacter* _Character);

	bool CheckConditions(ACharacter* _Character) const;

	virtual void TriggerSkill(ACharacter* _Character) PURE_VIRTUAL(...)

protected:
	UPROPERTY(EditDefaultsOnly)
		TObjectPtr<class UAnimMontage> Animation_;

	//Skill을 발동시키는 조건
	UPROPERTY()
		TScriptInterface<class IITrigger> SkillTrigger_;

	//Skill 발동에 필요한 조건들
	UPROPERTY()
		TArray<TObjectPtr<class IICondition>> Conditions_;

private:
	UPROPERTY(EditDefaultsOnly)
		FName SKillName_;

	UPROPERTY(EditDefaultsOnly)
		uint8 Damage_;

	//if Count is Inf, Set -1
	UPROPERTY(EditDefaultsOnly)
		int RemainingUses_;
};


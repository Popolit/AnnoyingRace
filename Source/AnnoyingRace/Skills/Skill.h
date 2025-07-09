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

	virtual bool IsSupportedForNetworking() const override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//NewObject<>(SkillTrigger)를 해 줄 것.
	//자식의 Initialize가 끝난 후 Super()를 호출할 것. 
	virtual void Initialize(ACharacter* _Character);

	uint8 GetDamage() const;

	virtual void TryTriggerSkill(ACharacter* _Character);

protected:
	bool CheckConditions(ACharacter* _Character) const;

	virtual void TriggerSkill(ACharacter* _Character) PURE_VIRTUAL(...)

protected:
	UPROPERTY(EditDefaultsOnly)
		TObjectPtr<class UAnimMontage> Animation_;

	//Skill을 발동시키는 조건, 비우면 트리거가 없는 스킬
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UTrigger> SkillTriggerClass_;

	UPROPERTY()
		TObjectPtr<UTrigger> SkillTrigger_;;

	//Skill 발동에 필요한 조건들
	UPROPERTY(Replicated)
		TArray<TObjectPtr<class UCondition_IdleState>> Conditions_;

private:
	UPROPERTY(EditDefaultsOnly)
		FName SKillName_;

	UPROPERTY(EditDefaultsOnly)
		uint8 Damage_;

	//if Count is Inf, Set -1
	UPROPERTY(EditDefaultsOnly, Replicated)
		int RemainingUses_;
};


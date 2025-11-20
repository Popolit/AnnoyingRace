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

	virtual void Tick(float _DeltaTime);
	
	virtual bool IsSupportedForNetworking() const override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//NewObject<>(SkillTrigger)를 해 줄 것.
	//자식의 Initialize가 끝난 후 Super()를 호출할 것. 
	virtual void Initialize(ACharacter* _Character);

	uint8 GetSkillDamage() const;

	virtual void TryTriggerSkill(ACharacter* _Character);

	TSoftObjectPtr<UTexture2D> GetSkillImg();

	int32 GetSkillCount() const;

	float GetSkillCoolDownRatio() const;
	
protected:
	bool CheckConditions(ACharacter* _Character) const;

	virtual void TriggerSkill(ACharacter* _Character);

	virtual void CancelSkill(ACharacter* _Character) PURE_VIRTUAL(...)
	
	void PlayAnimMontage(ACharacter* _Character) const;

protected:
	UPROPERTY(EditDefaultsOnly, Instanced)
		TObjectPtr<class UTrigger> SkillTrigger_;

	//Skill 발동에 필요한 조건, 비우면 조건 없는 스킬
	UPROPERTY(EditDefaultsOnly, Instanced)
		TObjectPtr<class UCondition> SkillCondition_;

private:
	UPROPERTY(EditDefaultsOnly)
		FName SKillName_;

	UPROPERTY(EditDefaultsOnly)
		uint8 Damage_;

	UPROPERTY(EditDefaultsOnly)
		TSoftObjectPtr<UTexture2D> SkillImg_;
	
	UPROPERTY(EditDefaultsOnly)
		TObjectPtr<class UAnimMontage> Animation_;

	//if Count is Inf, Set -1
	UPROPERTY(EditDefaultsOnly, Replicated)
		int32 RemainingUses_;

	UPROPERTY(EditDefaultsOnly)
		float SkillCoolDown_;

	float SkillCoolDownLeft_ = 0.f;
};


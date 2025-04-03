#pragma once

#include "CoreMinimal.h"
#include "Skill.generated.h"

/**
 * Skills UObject
 */
UCLASS(Blueprintable, HideDropdown)
class ANNOYINGRACE_API USkill : public UObject
{
	GENERATED_BODY()

public:
	USkill();

	void PushSkillButton();

private:
	UPROPERTY(EditDefaultsOnly)
		FName SKillName_;

	//if Count is Inf, Set -1
	UPROPERTY(EditDefaultsOnly)
		int Count_;

	UPROPERTY(EditDefaultsOnly)
		TSoftObjectPtr<class UAnimMontage> Animation_;
/*	//Push Skill Button
	void Skill(class APlayableCharacter* _Caster);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Skill")
		bool IsSkillReady();

	UFUNCTION(BlueprintNativeEvent, Category = "Skill")
		void ActivateSkill(APlayableCharacter* _Caster);
	virtual void ActivateSkill_Implementation(APlayableCharacter* _Caster);
	*/
};


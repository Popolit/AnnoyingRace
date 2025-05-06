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

	void PushSkillButton(class PlayableCharacter* _PC) {}

	void ReleaseSkillButton(class PlayableCharacter* _PC) {}

protected:
	virtual void UseSkill(class PlayableCharacter* _PC) {}

	bool CheckConditions() const;

protected:
	UPROPERTY(EditDefaultsOnly)
		TSoftObjectPtr<class UAnimMontage> Animation_;

private:
	UPROPERTY(EditDefaultsOnly)
		FName SKillName_;

	//if Count is Inf, Set -1
	UPROPERTY(EditDefaultsOnly)
		int RemainingUses_;

	TArray<TSharedPtr<class IICondition>> Conditions_;
};


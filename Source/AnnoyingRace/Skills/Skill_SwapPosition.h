#pragma once

#include "CoreMinimal.h"
#include "Skills/Skill_Target.h"
#include "Skill_SwapPosition.generated.h"

/**
 * Skill - Swap Position
 */
UCLASS(Blueprintable)
class ANNOYINGRACE_API USkill_SwapPosition : public USkill_Target
{
	GENERATED_BODY()
	
protected:
	virtual void Initialize(ACharacter* _Character) override;
	
	virtual void TriggerSkill(ACharacter* _Character) override;

	virtual void CancelSkill(ACharacter* _Character) override;
	
private:
	void ToggleAimWidget(ACharacter* _Character);


private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUserWidget> AimWidgetClass_;

	UPROPERTY()
		TObjectPtr<UUserWidget> AimWidget_;

	UPROPERTY(EditDefaultsOnly)
		float CharacterOpacityWhenAiming_ = 0.2f;
	
private:
	bool bCasting_ = false;

	TObjectPtr<ACharacter> CastedCharacter_;
};

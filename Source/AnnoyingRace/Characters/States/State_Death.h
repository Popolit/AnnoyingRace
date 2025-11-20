#pragma once

#include "CoreMinimal.h"
#include "Characters/States/State.h"
#include "State_Death.generated.h"

/**
 * State On Death
 * Make Character Ragdoll.
 */
UCLASS()
class ANNOYINGRACE_API UState_Death : public UState
{
	GENERATED_BODY()

public:
	virtual void Enter(UStateComponent* _Comp) override {}
	virtual void Exit(UStateComponent* _Comp) override {}
	virtual void Move(const FInputActionInstance& _Instance, ACharacter* _Character) override {}
	virtual void Look(const FInputActionInstance& _Instance, ACharacter* _Character) override {}
	virtual void SkillButtonPushed(const FInputActionInstance& _Instance, ACharacter* _Character) override {}
	virtual void CancelSkillButtonPushed(const struct FInputActionInstance& _Instance, ACharacter* _Character) override {}
	virtual void TakeDamage(ACharacter* _Character, float _DamageAmount, FDamageEvent const& _DamageEvent, AController* _EventInstigator, AActor* _DamageCauser) override;
};

#pragma once

#include "CoreMinimal.h"
#include "State.h"
#include "State_Casting.generated.h"

/**
 * Hit State
 */
UCLASS()
class ANNOYINGRACE_API UState_Casting : public UState
{
	GENERATED_BODY()

public:	
	virtual void Enter(class UStateComponent* _Comp) override;
	virtual void Exit(UStateComponent* _Comp) override;

	virtual void Move(const FInputActionInstance& _Instance, ACharacter* _Character) override;
	virtual void Look(const FInputActionInstance& _Instance, ACharacter* _Character) override;
	virtual void SkillButtonPushed(const FInputActionInstance& _Instance, ACharacter* _Character) override;
	virtual void CancelSkillButtonPushed(const struct FInputActionInstance& _Instance, ACharacter* _Character) override;
	
	virtual void TakeDamage(ACharacter* _Character, float _DamageAmount, FDamageEvent const& _DamageEvent, AController* _EventInstigator, AActor* _DamageCauser) override;
};

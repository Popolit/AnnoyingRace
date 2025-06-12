// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State.h"
#include "State_Hit.generated.h"

/**
 * Hit State
 */
UCLASS()
class ANNOYINGRACE_API UState_Hit : public UState
{
	GENERATED_BODY()

public:	
	virtual void Enter(class UStateComponent* _Comp) override;
	virtual void Exit(UStateComponent* _Comp) override;

	virtual void Move(const FInputActionInstance& _Instance, ACharacter* _Character) override;
	virtual void Look(const FInputActionInstance& _Instance, ACharacter* _Character) override;
	virtual void SkillButtonPushed(const FInputActionInstance& _Instance, ACharacter* _Character) override;

	virtual void TakeDamage(ACharacter* _Character, float _DamageAmount, FDamageEvent const& _DamageEvent, AController* _EventInstigator, AActor* _DamageCauser) override;
};

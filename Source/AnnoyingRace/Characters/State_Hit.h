// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/State.h"
#include "State_Hit.generated.h"

/**
 * 
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
	virtual void UseSkill(const FInputActionInstance& _Instance, ACharacter* _Character) override;
};

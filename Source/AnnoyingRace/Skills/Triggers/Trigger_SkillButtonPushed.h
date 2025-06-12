// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ITrigger.h"
#include "UObject/NoExportTypes.h"
#include "Trigger_SkillButtonPushed.generated.h"


/**
 * When Skill Button was Pushed
 */
UCLASS()
class ANNOYINGRACE_API UTrigger_SkillButtonPushed : public UObject, public IITrigger
{
	GENERATED_BODY()

public:
	virtual void Bind(ACharacter* _Character, TFunction<void(ACharacter*)> _Func) override;
	virtual void Triggered(ACharacter* _PC) override;

private:
	TFunction<void(ACharacter*)> Function_;
};

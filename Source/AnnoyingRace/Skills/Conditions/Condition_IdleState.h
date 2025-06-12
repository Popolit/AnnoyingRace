// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ICondition.h"
#include "UObject/NoExportTypes.h"
#include "Condition_IdleState.generated.h"

/**
 * IdleState
 */
UCLASS()
class ANNOYINGRACE_API UCondition_IdleState : public UObject, public IICondition
{
	GENERATED_BODY()
public:
	virtual bool CheckCondition(UObject* _Target) const override;
};

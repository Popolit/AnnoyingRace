// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICondition.generated.h"

UINTERFACE(MinimalAPI)
class UICondition : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ANNOYINGRACE_API IICondition
{
	GENERATED_BODY()

public:
	virtual bool Check() = 0;
};

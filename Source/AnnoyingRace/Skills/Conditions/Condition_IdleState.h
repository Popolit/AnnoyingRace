#pragma once

#include "CoreMinimal.h"
#include "Condition.h"
#include "Condition_IdleState.generated.h"

/**
 * Skill Condition IdleState
 */
UCLASS()
class ANNOYINGRACE_API UCondition_IdleState : public UCondition
{
	GENERATED_BODY()
public:
	virtual bool CheckCondition(UObject* _Target) const override;
};

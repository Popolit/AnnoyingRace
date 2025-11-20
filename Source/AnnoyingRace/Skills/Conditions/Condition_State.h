#pragma once

#include "CoreMinimal.h"
#include "Condition.h"
#include "Components/StateComponent.h"
#include "Condition_State.generated.h"

/**
 * Skill Condition Specific State
 */
UCLASS()
class ANNOYINGRACE_API UCondition_State : public UCondition
{
	GENERATED_BODY()
public:
	virtual bool CheckCondition(UObject* _Target) const override;

private:
	UPROPERTY(EditDefaultsOnly)
		EState StateToCheck_;
};

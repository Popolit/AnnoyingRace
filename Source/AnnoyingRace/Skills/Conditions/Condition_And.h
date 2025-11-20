#pragma once

#include "CoreMinimal.h"
#include "Skills/Conditions/Condition.h"
#include "Condition_And.generated.h"

/**
 * Condition -> And (Left and Right)
 */
UCLASS()
class ANNOYINGRACE_API UCondition_And : public UCondition
{
	GENERATED_BODY()

public:
	virtual bool CheckCondition(UObject* _Target) const override;

private:
	UPROPERTY(EditAnywhere, Instanced, meta = (DisplayName = "Condition_A"))
		TObjectPtr<UCondition> Condition_Left_;
	
	UPROPERTY(EditAnywhere, Instanced, meta = (DisplayName = "Condition_B"))
		TObjectPtr<UCondition> Condition_Right_;
};

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Condition.generated.h"

/**
 * Skill Conditions
 */
UCLASS(Abstract, HideDropDown, EditInlineNew)
class ANNOYINGRACE_API UCondition : public UObject
{
	GENERATED_BODY()
public:
	virtual bool IsSupportedForNetworking() const override;

	virtual bool CheckCondition(UObject* _Target) const PURE_VIRTUAL(UCondition::CheckCondition, return false;);
};

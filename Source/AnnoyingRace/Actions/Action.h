#pragma once

#include "CoreMinimal.h"
#include "IAction.h"
#include "Action.generated.h"

/*
 * Actions with Command Pattern 
 */
UCLASS(HideDropdown)
class ANNOYINGRACE_API UAction : public UObject, public IIAction
{
	GENERATED_BODY()
public:
	virtual void DoAction() override;
};

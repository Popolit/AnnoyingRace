#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IAction.generated.h"

UINTERFACE(MinimalAPI)
class UIAction : public UInterface
{
	GENERATED_BODY()
};

class ANNOYINGRACE_API IIAction
{
	GENERATED_BODY()
public:
	virtual void DoAction() = 0;
};

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
 * Various Conditions (Ex : HP < 50%)
 */
class ANNOYINGRACE_API IICondition
{
	GENERATED_BODY()

public:

	virtual bool CheckCondition(UObject* _Target) const = 0;
};

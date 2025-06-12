#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ITrigger.generated.h"

UINTERFACE(MinimalAPI)
class UITrigger : public UInterface
{
	GENERATED_BODY()
};

/**
 * Some Condition was Triggered
 */
class ANNOYINGRACE_API IITrigger
{
	GENERATED_BODY()

public:
	virtual void Bind(ACharacter* _Character, TFunction<void(ACharacter*)> _Func) = 0;
	virtual void Triggered(ACharacter* _PC) = 0;
};

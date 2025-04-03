#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "Action_Move.generated.h"

/**
 * 캐릭터 이동
 */
UCLASS()
class ANNOYINGRACE_API UAction_Move : public UAction
{
	GENERATED_BODY()

public:
	virtual void DoAction() override;
};

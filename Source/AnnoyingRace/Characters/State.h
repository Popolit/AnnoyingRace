#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "State.generated.h"

/**
 * StateBase Class
 */
UCLASS()
class ANNOYINGRACE_API UState : public UObject
{
	GENERATED_BODY()
	
public:
	UState() {}
	
	virtual void Enter(class UStateComponent* _Comp) PURE_VIRTUAL(...);
	virtual void Exit(UStateComponent* _Comp) PURE_VIRTUAL(...);

	virtual void Move(const struct FInputActionInstance& _Instance, ACharacter* _Character) PURE_VIRTUAL(...);
	virtual void Look(const struct FInputActionInstance& _Instance, ACharacter* _Character) PURE_VIRTUAL(...);
	virtual void UseSkill(const struct FInputActionInstance& _Instance, ACharacter* _Character) PURE_VIRTUAL(...);
};

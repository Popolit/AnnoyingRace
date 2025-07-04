#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RaceSpectatorPawn.generated.h"

/* 
 * Default Pawn of Game (Only Has Camera)
 */
UCLASS()
class ANNOYINGRACE_API ARaceSpectatorPawn : public APawn
{
	GENERATED_BODY()

public:
	ARaceSpectatorPawn();

private:
	UPROPERTY()
		TObjectPtr<class USpringArmComponent> SpringArmComponent_;
	UPROPERTY()
		TObjectPtr<class UCameraComponent> CameraComponent_;
};

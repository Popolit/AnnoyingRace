#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "RaceSpectatorPawn.generated.h"

/* 
 * Pawn for Spectating
 */
UCLASS(HideDropdown)
class ANNOYINGRACE_API ARaceSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()

private:
	virtual void SetupPlayerInputComponent(UInputComponent* _PlayerInputComponent) override;

	void Move(const struct FInputActionInstance& _Instance);
	void Look(const struct FInputActionInstance& _Instance);

	//Inputs
private:
	UPROPERTY(EditDefaultsOnly, Category = Input)
		TObjectPtr<class UInputAction> IA_Move_;
	UPROPERTY(EditDefaultsOnly, Category = Input)
		TObjectPtr<class UInputAction> IA_Look_;
};

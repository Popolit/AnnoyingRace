#include "RaceSpectatorPawn.h"

#include "EnhancedInputComponent.h"
#include "InputTriggers.h"



void ARaceSpectatorPawn::SetupPlayerInputComponent(UInputComponent* _PlayerInputComponent)
{
	UEnhancedInputComponent* IC = Cast<UEnhancedInputComponent>(_PlayerInputComponent);
	check(IC);

	if (ensureMsgf(IA_Move_, TEXT("%s's IA_Move was nullptr"), *GetName()))
	{
		IC->BindAction(IA_Move_, ETriggerEvent::Triggered, this, &ARaceSpectatorPawn::Move);
	}
	if (ensureMsgf(IA_Look_, TEXT("%s's IA_Look was nullptr"), *GetName()))
	{
		IC->BindAction(IA_Look_, ETriggerEvent::Triggered, this, &ARaceSpectatorPawn::Look);
	}
}

void ARaceSpectatorPawn::Move(const FInputActionInstance& _Instance)
{
	const FVector2D MovingFactor = _Instance.GetValue().Get<FVector2D>();

	FRotator const ControlRotation = GetControlRotation();

	auto X = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::X) * MovingFactor.X;
	auto Y = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y) * MovingFactor.Y;
	AddMovementInput(X + Y);
}

void ARaceSpectatorPawn::Look(const FInputActionInstance& _Instance)
{
	const FVector2D LookingFactor = _Instance.GetValue().Get<FVector2D>();
	AddControllerYawInput(LookingFactor.X);
	AddControllerPitchInput(-LookingFactor.Y);
}

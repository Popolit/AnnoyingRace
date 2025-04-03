#include "PlayableCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AnnoyingRace/Skills/Skill.h"


APlayableCharacter::APlayableCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CreateAllComponents();

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	SpringArmComponent_->SetRelativeLocation(FVector(0.0, 0.0, 150.0));
	SpringArmComponent_->SetRelativeRotation(FRotator(0.0, 90.0, 0.0));
	SpringArmComponent_->TargetArmLength = 400.0;
	SpringArmComponent_->bUsePawnControlRotation = true;

	CameraComponent_->SetRelativeLocation(FVector(-30.0, 0.0, 0.0));
}

void APlayableCharacter::SetupPlayerInputComponent(UInputComponent* _PlayerInputComponent)
{
	UEnhancedInputComponent* IC = Cast<UEnhancedInputComponent>(_PlayerInputComponent);
	check(IC);

	
	if (ensureMsgf(IMC_, TEXT("%s's Input Mapping Context was nullptr"), *GetName()))
	{
		if(const APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				SubSystem->AddMappingContext(IMC_, 0);
			}
		}


		IC->BindAction(IA_Move_, ETriggerEvent::Triggered, this, &APlayableCharacter::Move);
		if (ensureMsgf(IA_Move_, TEXT("%s's IA_Move was nullptr"), *GetName()))
		{
			IC->BindAction(IA_Move_, ETriggerEvent::Triggered, this, &APlayableCharacter::Move);
		}
		if (ensureMsgf(IA_Look_, TEXT("%s's IA_Look was nullptr"), *GetName()))
		{
			IC->BindAction(IA_Look_, ETriggerEvent::Triggered, this, &APlayableCharacter::Look);
		}
		if (ensureMsgf(IA_UseSkill_, TEXT("%s's IA_UseSkill was nullptr"), *GetName()))
		{
			IC->BindAction(IA_UseSkill_, ETriggerEvent::Triggered, this, &APlayableCharacter::UseSkill);
		}
	}
}

void APlayableCharacter::CreateAllComponents()
{
	SpringArmComponent_ = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent_->SetupAttachment(GetMesh());

	CameraComponent_ = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent_->SetupAttachment(SpringArmComponent_.Get());
}

void APlayableCharacter::Move(const FInputActionInstance& _Instance)
{
	const FVector2D MovingFactor = _Instance.GetValue().Get<FVector2D>();
	FVector Direction = FVector(MovingFactor.X, MovingFactor.Y, 0.0f);

	const FRotator CharacterRotation = GetControlRotation();
	Direction = CharacterRotation.RotateVector(Direction);

	AddMovementInput(Direction.GetSafeNormal2D());
}

void APlayableCharacter::Look(const FInputActionInstance& _Instance)
{
	const FVector2D LookingFactor = _Instance.GetValue().Get<FVector2D>();
	AddControllerYawInput(LookingFactor.X);
	AddControllerPitchInput(LookingFactor.Y);
}

void APlayableCharacter::UseSkill(const FInputActionInstance& _Instance)
{
	if (false == Skill_.IsNull())
	{
		//Skill_->Skill(this);
	}
}


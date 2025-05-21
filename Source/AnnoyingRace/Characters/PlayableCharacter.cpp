#include "PlayableCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StateComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


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

		if (ensureMsgf(IA_Move_, TEXT("%s's IA_Move was nullptr"), *GetName()))
		{
			IC->BindAction(IA_Move_, ETriggerEvent::Triggered, StateComponent_.Get(), &UStateComponent::Move);
		}
		if (ensureMsgf(IA_Look_, TEXT("%s's IA_Look was nullptr"), *GetName()))
		{
			IC->BindAction(IA_Look_, ETriggerEvent::Triggered, StateComponent_.Get(), &UStateComponent::Look);
		}
		if (ensureMsgf(IA_UseSkill_, TEXT("%s's IA_UseSkill was nullptr"), *GetName()))
		{
			IC->BindAction(IA_UseSkill_, ETriggerEvent::Triggered, StateComponent_.Get(), &UStateComponent::UseSkill);
		}
	}
}

void APlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

	StateComponent_->BeginPlay();
}

void APlayableCharacter::CreateAllComponents()
{
	SpringArmComponent_ = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent_->SetupAttachment(GetMesh());

	CameraComponent_ = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent_->SetupAttachment(SpringArmComponent_.Get());

	StateComponent_ = CreateDefaultSubobject<UStateComponent>("State");
}
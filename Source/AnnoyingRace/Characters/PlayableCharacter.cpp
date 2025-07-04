#include "PlayableCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StateComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SkillComponent.h"


APlayableCharacter::APlayableCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicateMovement(true);

	CreateAllComponents();

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	SpringArmComponent_->SetRelativeLocation(FVector(0.0, 0.0, 150.0));
	SpringArmComponent_->SetRelativeRotation(FRotator(0.0, 90.0, 0.0));
	SpringArmComponent_->TargetArmLength = 400.0;
	SpringArmComponent_->bUsePawnControlRotation = true;

	CameraComponent_->SetRelativeLocation(FVector(-30.0, 0.0, 0.0));
	HP_ = 1;
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
			IC->BindAction(IA_UseSkill_, ETriggerEvent::Triggered, StateComponent_.Get(), &UStateComponent::SkillButtonPushed);
		}
	}
}

float APlayableCharacter::TakeDamage(float _DamageAmount, FDamageEvent const& _DamageEvent, AController* _EventInstigator, AActor* _DamageCauser)
{
	StateComponent_->TakeDamage(this, _DamageAmount, _DamageEvent, _EventInstigator, _DamageCauser);
	return _DamageAmount;
}


void APlayableCharacter::ProcessHit(uint8 _DamageAmount, FDamageEvent const& _DamageEvent)
{
	if (HP_ < _DamageAmount)
	{
		HP_ = 0;
		ProcessDeath();
		return;
	}

	HP_ -= _DamageAmount;
	if (ensureMsgf(HitAnimation_, TEXT("%s's DeathAnimation was nullptr"), *GetName()))
	{
		PlayAnimMontage(HitAnimation_);
	}
	StateComponent_->SetState(EState::Hit);
}

void APlayableCharacter::ProcessDeath()
{
	if(HasAuthority())
	{
		OnCharacterDied_.ExecuteIfBound(this);
		if (ensureMsgf(DeathAnimation_, TEXT("%s's DeathAnimation was nullptr"), *GetName()))
		{
			PlayAnimMontage(DeathAnimation_);
		}
	}
}

void APlayableCharacter::SkillButtonPushed()
{
	if(HasAuthority())
	{
		if(OnSkillButtonPushed_.IsBound())
		{
			UE_LOG(LogTemp, Warning, TEXT("SERVER: Bound"));
		}
	}
	else if(OnSkillButtonPushed_.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("CLIENT: Bound"));
	}

	OnSkillButtonPushed_.ExecuteIfBound(this);
}


void APlayableCharacter::CreateAllComponents()
{
	SpringArmComponent_ = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent_->SetupAttachment(GetMesh());

	CameraComponent_ = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent_->SetupAttachment(SpringArmComponent_);

	StateComponent_ = CreateDefaultSubobject<UStateComponent>("State");
	SkillComponent_ = CreateDefaultSubobject<USkillComponent>("Skill");
}

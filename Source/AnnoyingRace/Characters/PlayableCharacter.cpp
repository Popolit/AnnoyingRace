#include "PlayableCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StateComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SkillComponent.h"
#include "Components/StatusComponent.h"


APlayableCharacter::APlayableCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicateMovement(true);

	CreateAllComponents();
	
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->MaxWalkSpeed = StatusComponent_->GetSpeed();
	MovementComponent->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	SpringArmComponent_->SetRelativeLocation(FVector(0.0, 0.0, 150.0));
	SpringArmComponent_->SetRelativeRotation(FRotator(0.0, 90.0, 0.0));
	SpringArmComponent_->TargetArmLength = 400.0;
	SpringArmComponent_->bUsePawnControlRotation = true;

	CameraComponent_->SetRelativeLocation(FVector(-30.0, 0.0, 0.0));

	ObservingCamera_->SetRelativeLocation(FVector(-230, 0, 600));
	ObservingCamera_->SetRelativeRotation(FRotator(-80, 0, 0));

	LifeSpanAfterDeath = 3.f;
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
	StatusComponent_->DamageToHP(_DamageAmount);

	//사망 했으면 히트처리하지 않음
	if(StateComponent_->CheckCurrentState(EState::Death))
	{
		return;
	}

	if(HitAnimation_)
	{
		StateComponent_->SetState(EState::Hit);
		Server_PlayAnimMontage(HitAnimation_);
	}
}


//void APlayableCharacter::ProcessDeath()
//{
//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//	check(AnimInstance)
//	FOnMontageEnded OnMontageEnded;
//	OnMontageEnded.BindUObject(this, &APlayableCharacter::OnDeathAnimationEnded);
//
//	AnimInstance->Montage_SetEndDelegate(OnMontageEnded, DeathAnimation_);
//
//	Server_PlayAnimMontage(DeathAnimation_);
//}

void APlayableCharacter::SkillButtonPushed()
{
	OnSkillButtonPushed_.ExecuteIfBound(this);
}

void APlayableCharacter::Multicast_ProcessDeath_Implementation()
{
	StateComponent_->SetState(EState::Death);

	USkeletalMeshComponent* MeshComponent = GetMesh();
	check(MeshComponent);
	MeshComponent->SetCollisionProfileName(FName("Ragdoll"));
	MeshComponent->SetSimulatePhysics(true);
	SetLifeSpan(3.f);

	if(CameraComponent_)
	{
		CameraComponent_->Deactivate();
	}
	if (ObservingCamera_)
	{
		ObservingCamera_->Activate();
	}
}

void APlayableCharacter::Server_PlayAnimMontage_Implementation(UAnimMontage* _Montage)
{
	if(_Montage)
	{
		Multicast_PlayAnimMontage(_Montage);
	}
}

void APlayableCharacter::Multicast_PlayAnimMontage_Implementation(UAnimMontage* _Montage)
{
	PlayAnimMontage(_Montage);
}


void APlayableCharacter::CreateAllComponents()
{
	SpringArmComponent_ = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent_->SetupAttachment(GetMesh());

	CameraComponent_ = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent_->SetupAttachment(SpringArmComponent_);

	ObservingCamera_ = CreateDefaultSubobject<UCameraComponent>("Observing Camera");
	ObservingCamera_->SetupAttachment(GetRootComponent());

	StateComponent_ = CreateDefaultSubobject<UStateComponent>("State");
	StatusComponent_ = CreateDefaultSubobject<UStatusComponent>("Status");
	SkillComponent_ = CreateDefaultSubobject<USkillComponent>("Skill");
}


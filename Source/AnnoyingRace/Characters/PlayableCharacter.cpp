#include "PlayableCharacter.h"

#include "RaceGameMode.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StateComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "RacePlayerController.h"
#include "Components/AudioComponent.h"
#include "Components/SkillComponent.h"
#include "Components/StatusComponent.h"
#include "Skills/RaceDamageEvent.h"


APlayableCharacter::APlayableCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicateMovement(true);

	CreateAllComponents();
	
	bUseControllerRotationYaw = false;

	SpringArmComponent_->SetRelativeLocation(FVector(0.0, 0.0, 150.0));
	SpringArmComponent_->SetRelativeRotation(FRotator(0.0, 90.0, 0.0));
	SpringArmComponent_->TargetArmLength = 400.0;
	SpringArmComponent_->bUsePawnControlRotation = true;

	CameraComponent_->SetRelativeLocation(FVector(-30.0, 0.0, 0.0));

	ObservingCamera_->SetRelativeLocation(FVector(-230, 0, 600));
	ObservingCamera_->SetRelativeRotation(FRotator(-80, 0, 0));

	LifeSpanAfterDeath_ = 3.f;
}

void APlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->MaxWalkSpeed = StatusComponent_->GetSpeed();
	MovementComponent->bOrientRotationToMovement = true;
}

void APlayableCharacter::SetupPlayerInputComponent(UInputComponent* _PlayerInputComponent)
{
	UEnhancedInputComponent* IC = Cast<UEnhancedInputComponent>(_PlayerInputComponent);
	check(IC);

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

void APlayableCharacter::LaunchCharacter(FVector _LaunchVelocity, bool _bXYOverride, bool _bZOverride)
{
	Server_LaunchCharacter(_LaunchVelocity , _bXYOverride , _bZOverride);
}

float APlayableCharacter::TakeDamage(float _DamageAmount, FDamageEvent const& _DamageEvent, AController* _EventInstigator, AActor* _DamageCauser)
{
	StateComponent_->TakeDamage(this, _DamageAmount, _DamageEvent, _EventInstigator, _DamageCauser);
	return _DamageAmount;
}


void APlayableCharacter::ProcessHit(uint8 _DamageAmount, FDamageEvent const& _DamageEvent)
{
	StatusComponent_->DamageToHP(_DamageAmount);
	const FRaceDamageEvent* RaceDamageEvent = static_cast<const FRaceDamageEvent*>(&_DamageEvent);

	if (RaceDamageEvent)
	{
		LaunchCharacter(RaceDamageEvent->KnockbackVector_ , true, false);
	}
	
	//사망했는지 확인
	if(StateComponent_->CheckCurrentState(EState::Death))
	{
		return;
	}

	if(HitAnimation_)
	{
		//StateComponent_->SetState(EState::Hit);
		Server_PlayAnimMontage(HitAnimation_);
	}
}

void APlayableCharacter::SkillButtonPushed()
{
	OnSkillButtonPushed_.ExecuteIfBound(this);
}

//장외로 떨어질 시 사망 처리
void APlayableCharacter::FellOutOfWorld(const UDamageType& _DmgType)
{
	if (HasAuthority() && false == StateComponent_->CheckCurrentState(EState::Death))
	{
		ProcessDeath();
	}
}

void APlayableCharacter::ProcessDeath()
{
	if (HasAuthority())
	{
		ARacePlayerController* PC = Cast <ARacePlayerController>(GetController());
		check(PC);

		if (ARaceGameMode* GM = GetWorld()->GetAuthGameMode<ARaceGameMode>())
		{
			PC->SetSpectatorMode(CameraComponent_->GetComponentTransform());
			GM->HandlePlayerDeath(PC);
		}
		Multicast_ProcessDeath();
	}
}


void APlayableCharacter::Server_PlayAnimMontage_Implementation(UAnimMontage* _Montage)
{
	if(_Montage)
	{
		Multicast_PlayAnimMontage(_Montage);
	}
}

void APlayableCharacter::Multicast_ProcessDeath_Implementation()
{
	StateComponent_->SetState(EState::Death);

	USkeletalMeshComponent* MeshComponent = GetMesh();
	check(MeshComponent);
	MeshComponent->SetCollisionProfileName(TEXT("Ragdoll"));
	MeshComponent->SetSimulatePhysics(true);
	SetLifeSpan(LifeSpanAfterDeath_);

	if(ARacePlayerController* PC = Cast<ARacePlayerController>(GetController()))
	{
		PC->SetSpectatorMode(CameraComponent_->GetComponentTransform());
	}
	OnCharacterDied_.ExecuteIfBound(this);
}

void APlayableCharacter::Multicast_PlayAnimMontage_Implementation(UAnimMontage* _Montage)
{
	PlayAnimMontage(_Montage);
}


void APlayableCharacter::Server_LaunchCharacter_Implementation(const FVector& _LaunchVelocity, bool _bXYOverride,
	bool _bZOverride)
{
	Super::LaunchCharacter(_LaunchVelocity, _bXYOverride, _bZOverride);
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


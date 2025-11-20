#include "SkillComponent.h"

#include "Engine/ActorChannel.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Skills/Skill_Target.h"


USkillComponent::USkillComponent()
{
	SetIsReplicatedByDefault(true);
	PrimaryComponentTick.bCanEverTick = true;
}

void USkillComponent::TickComponent(float _DeltaTime, enum ELevelTick _TickType,
	FActorComponentTickFunction* _ThisTickFunction)
{
	Super::TickComponent(_DeltaTime , _TickType , _ThisTickFunction);
	if (Skill_)
	{
		Skill_->Tick(_DeltaTime);
	}
}

void USkillComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USkillComponent, Skill_);
}

bool USkillComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bResult = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	bResult |= Channel->ReplicateSubobject(Skill_, *Bunch, *RepFlags);

	return bResult;
}

void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	check(Character);

	if (Character->HasAuthority())
	{
		if (SkillClass_)
		{
			Skill_ = NewObject<USkill>(this, SkillClass_);
			OnRep_Skill();
		}
	}
}

uint8 USkillComponent::GetSkillDamage() const
{
	if (Skill_)
	{
		return Skill_->GetSkillDamage();
	}
	return 0;
}

void USkillComponent::GetSkillTargets(TArray<AActor*>& _OutTargets) const
{
	auto TargetingSkill = Cast<USkill_Target>(Skill_);
	if (TargetingSkill)
	{
		TargetingSkill->GetTargets(_OutTargets);
	}
}

TSoftObjectPtr<UTexture2D> USkillComponent::GetSkillImg()
{
	if (Skill_)
	{
		return Skill_->GetSkillImg();
	}
	return nullptr;
}

int32 USkillComponent::GetSkillCount() const
{
	if (Skill_)
	{
		return Skill_->GetSkillCount();
	}
	return 0;
}

float USkillComponent::GetSkillCoolDownRatio() const
{
	if (Skill_)
	{
		return Skill_->GetSkillCoolDownRatio();
	}
	return 0;
}

void USkillComponent::OnRep_Skill()
{
	if (Skill_)
	{
		ACharacter* Character = Cast<ACharacter>(GetOwner());
		check(Character);

		Skill_->Initialize(Character);
		Skill_->GetSkillImg();
	}
}



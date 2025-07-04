#include "SkillComponent.h"

#include "Engine/ActorChannel.h"
#include "Skills/Skill.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"


USkillComponent::USkillComponent()
{
	SetIsReplicatedByDefault(true);
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
		check(SkillClass_);
		Skill_ = NewObject<USkill>(this, SkillClass_);
		OnRep_Skill();
	}
}

uint8 USkillComponent::GetSkillDamage() const
{
	return Skill_->GetDamage();
}

void USkillComponent::OnRep_Skill()
{
	check(Skill_);

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	check(Character);

	Skill_->Initialize(Character);
}



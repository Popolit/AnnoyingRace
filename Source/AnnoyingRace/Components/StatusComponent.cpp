#include "StatusComponent.h"

#include "Characters/PlayableCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

UStatusComponent::UStatusComponent()
{
	HP_ = 100.f;
	BaseSpeed_ = 400.f;
	AddedSpeedBonus_ = 0.f;
	MultipliedSpeedBonus_ = 0.f;

	SetIsReplicatedByDefault(true);
}

void UStatusComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UStatusComponent, HP_);
	DOREPLIFETIME(UStatusComponent, AddedSpeedBonus_);
	DOREPLIFETIME(UStatusComponent, MultipliedSpeedBonus_);
}

float UStatusComponent::GetSpeed() const
{
	return (BaseSpeed_ + AddedSpeedBonus_) * (1.f + MultipliedSpeedBonus_);
}

void UStatusComponent::AddSpeedBonus(float _SpeedBonus)
{
	if (GetOwner()->HasAuthority())
	{
		AddedSpeedBonus_ += _SpeedBonus;
		OnRep_Speed();
	}
}

void UStatusComponent::MultiplySpeedBonus(float _SpeedBonus)
{
	if (GetOwner()->HasAuthority())
	{
		MultipliedSpeedBonus_ += _SpeedBonus;
		OnRep_Speed();
	}
}

void UStatusComponent::DamageToHP(float _Amount)
{
	APlayableCharacter* Character = Cast<APlayableCharacter>(GetOwner());
	if(ensureMsgf(Character, TEXT("%s's Owner Character was nullptr"), *GetName()))
	{
		if (Character->HasAuthority())
		{
			HP_ -= _Amount;

			if (HP_ <= 0.f)
			{
				HP_ = 0.f;
				Character->ProcessDeath();
			}
		}
	}
}


void UStatusComponent::OnRep_Speed()
{
	auto MovementComponent = Cast<UCharacterMovementComponent>(GetOwner()->GetComponentByClass(UCharacterMovementComponent::StaticClass()));

	MovementComponent->MaxWalkSpeed = GetSpeed();
}
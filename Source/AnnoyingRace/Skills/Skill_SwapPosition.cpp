#include "Skill_SwapPosition.h"

#include "Blueprint/UserWidget.h"
#include "Characters/PlayableCharacter.h"
#include "Components/StateComponent.h"

void USkill_SwapPosition::Initialize(ACharacter* _Character)
{
	Super::Initialize(_Character);

	auto PlayableCharacter = Cast<APlayableCharacter>(_Character);

	if (PlayableCharacter)
	{
		PlayableCharacter->OnCancelSkillButtonPushed_.BindUObject(this, &USkill_SwapPosition::CancelSkill);
	}
}

void USkill_SwapPosition::TriggerSkill(ACharacter* _Character)
{
	auto StateComp = _Character->GetComponentByClass<UStateComponent>();
	if (nullptr == StateComp)
	{
		return;
	}
	
	if (StateComp->CheckCurrentState(EState::Idle))
	{
		StateComp->SetState(EState::Casting);
		bCasting_ = true;
		
		ToggleAimWidget(_Character);
	}
	else if (StateComp->CheckCurrentState(EState::Casting))
	{
		TArray<AActor*> TargetActors;
		SetTargets(_Character);
		GetTargets(TargetActors);
		if(TargetActors.IsEmpty())
		{
			return;
		}

		auto TargetCharacter = Cast<APlayableCharacter>(TargetActors.Top());
		PlayAnimMontage(_Character);

		if (TargetCharacter)
		{
			auto TargetStateComp = TargetCharacter->GetComponentByClass<UStateComponent>();

			if (TargetStateComp)
			{
				TargetStateComp->SetState(EState::Incapacitated);
			}
		}
		bCasting_ = false;
		ToggleAimWidget(_Character);

		Super::TriggerSkill(_Character);
	}
}

void USkill_SwapPosition::CancelSkill(ACharacter* _Character)
{
	auto StateComp = _Character->GetComponentByClass<UStateComponent>();
	if (nullptr == StateComp || false == StateComp->CheckCurrentState(EState::Casting))
	{
		return;
	}
	
	StateComp->SetState(EState::Idle);
	bCasting_ = false;
	ToggleAimWidget(_Character);
}

void USkill_SwapPosition::ToggleAimWidget(ACharacter* _Character)
{
	auto PlayableCharacter = Cast<APlayableCharacter>(_Character);
	if (_Character->IsLocallyControlled())
	{
		if (bCasting_)
		{
			if (AimWidget_)
			{
				AimWidget_->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
			else if (ensureMsgf(AimWidgetClass_, TEXT("AimWidget of Skill SwapPosition was not set")))
			{
				AimWidget_ = CreateWidget(GetWorld(), AimWidgetClass_);
				AimWidget_->AddToViewport(1);
			}
			if (PlayableCharacter)
			{
				PlayableCharacter->SetOpacity(CharacterOpacityWhenAiming_);
			}
		}
		else if (AimWidget_)
		{
			AimWidget_->SetVisibility(ESlateVisibility::Collapsed);

			if (PlayableCharacter)
			{
				PlayableCharacter->SetOpacity(1.f);
			}
		}
	}
}
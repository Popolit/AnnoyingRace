#include "Skills/Trigger_SkillButtonPushed.h"

#include "GameFramework/Character.h"
#include "Components/SkillComponent.h"

void UTrigger_SkillButtonPushed::Bind(ACharacter* _Character, TFunction<void(ACharacter*)> _Func)
{
	USkillComponent* SkillComponent = Cast<USkillComponent>(_Character->GetComponentByClass(USkillComponent::StaticClass()));

	if (ensureMsgf(SkillComponent, TEXT("%s's SkillComponent was nullptr"), *_Character->GetName()))
	{
		SkillComponent->SetTrigger(_Func);
	}
}

void UTrigger_SkillButtonPushed::Triggered(ACharacter* _Character)
{
	if (Function_)
	{
		Function_(_Character);
	}
}

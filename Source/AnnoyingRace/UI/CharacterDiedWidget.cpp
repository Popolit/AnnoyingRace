#include "CharacterDiedWidget.h"

#include "Components/TextBlock.h"


void UCharacterDiedWidget::SetWidget()
{
	Count_ = 5;
	Txt_CountDown_->SetText(FText::AsNumber(Count_));
}

void UCharacterDiedWidget::ShowWidget()
{
	PlayAnimation(Anim_CountDown_);
}

void UCharacterDiedWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* _Animation)
{
	if(_Animation == Anim_CountDown_)
	{
		OnCharacterDIedAnimationFinished_.ExecuteIfBound();
	}
}

void UCharacterDiedWidget::UpdateCountDown()
{
	Txt_CountDown_->SetText(FText::AsNumber(--Count_));
}

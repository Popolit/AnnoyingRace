#include "CountDownWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"

void UCountDownWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimation(Anim_CountDown_);
}

void UCountDownWidget::UpdateCountDown()
{
	if(1 == CountDown_)
	{
		Txt_CountDown_->SetText(FText::FromString(TEXT("GO!")));

		APlayerController* PC = GetOwningPlayer();
		PC->EnableInput(PC);
		
		return;
	}
	Txt_CountDown_->SetText(FText::AsNumber(--CountDown_));
}

void UCountDownWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* _Animation)
{
	if(_Animation == Anim_CountDown_)
	{
		OnCountdownAnimationFinished_.ExecuteIfBound();
	}
}


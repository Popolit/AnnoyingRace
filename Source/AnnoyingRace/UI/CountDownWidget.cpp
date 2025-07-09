#include "CountDownWidget.h"

#include "TimerManager.h"
#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"

void UCountDownWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimation(AnimCountDown_);
	auto Bindings = AnimCountDown_->GetBindings();
}

void UCountDownWidget::UpdateCountDown()
{
	if(1 == CountDown_)
	{
		TxtCountDown_->SetText(FText::FromString(TEXT("GO!")));

		APlayerController* PC = GetOwningPlayer();
		PC->EnableInput(PC);
		
		return;
	}
	TxtCountDown_->SetText(FText::AsNumber(--CountDown_));
}

void UCountDownWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* _Animation)
{
	if(_Animation == AnimCountDown_)
	{
		OnCountdownAnimationFinished_.ExecuteIfBound();
	}
}


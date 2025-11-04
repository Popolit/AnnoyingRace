#include "RaceFinishedWidget.h"

#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

void URaceFinishedWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Count_ = 5;
	Txt_CountDown_->SetText(FText::AsNumber(Count_));
	PlayAnimation(Anim_CountDown_);
}

void URaceFinishedWidget::SetRank(const int32 _Rank)
{
	WST_Rank_->SetActiveWidgetIndex(_Rank - 1);
}

void URaceFinishedWidget::UpdateCountDown()
{
	Txt_CountDown_->SetText(FText::AsNumber(--Count_));
}

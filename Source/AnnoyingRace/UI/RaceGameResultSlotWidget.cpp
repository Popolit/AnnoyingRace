#include "RaceGameResultSlotWidget.h"

#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

void URaceGameResultSlotWidget::SetWidget(int32 _Rank, const FString& _PlayerName) const
{
	WST_PlayerRank_->SetActiveWidgetIndex(_Rank - 1);
	Txt_PlayerName_->SetText(FText::FromString(_PlayerName));
}

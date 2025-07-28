#include "RaceRankSlotWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

void URaceRankSlotWidget::NativeTick(const FGeometry& _MyGeometry, float _InDeltaTime)
{
	Super::NativeTick(_MyGeometry, _InDeltaTime);

	//Move Animation (Interp)
	if (false == FMath::IsNearlyEqual(CurrentYPosition_, TargetYPosition_))
	{
		CurrentYPosition_ = FMath::FInterpTo(CurrentYPosition_, TargetYPosition_, _InDeltaTime, 10.0f);
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
		{
			CanvasSlot->SetPosition(FVector2D(0, CurrentYPosition_));
		}
	}
}

void URaceRankSlotWidget::SetRank(uint8 _Rank)
{
	TargetYPosition_ = _Rank * GetDesiredSize().Y;
}


void URaceRankSlotWidget::SetImage(TSoftObjectPtr<UTexture2D> _Image)
{
	Img_UserCharacter_->SetBrushFromSoftTexture(_Image);
}

void URaceRankSlotWidget::SetText(const FString& _Text)
{
	//TODO : UserID¸¦ ¼ÂÆÃ
}

#include "RaceRankSlotWidget.h"

#include "Characters/CharacterData.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void URaceRankSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ForceLayoutPrepass();

	SizeY_ = GetDesiredSize().Y;
}

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

void URaceRankSlotWidget::SetPlayerName(const FString& _PlayerName) const
{
	Txt_PlayerName_->SetText(FText::FromString(_PlayerName));
}

void URaceRankSlotWidget::SetSlotFromCharacterData(const UCharacterData* _CharacterData)
{
	SetImage(_CharacterData->GetCharacterIcon());
}

void URaceRankSlotWidget::SetRank(uint8 _Rank)
{
	TargetYPosition_ = _Rank * SizeY_;
}


void URaceRankSlotWidget::SetImage(TSoftObjectPtr<UTexture2D> _Image) const
{
	Img_UserCharacter_->SetBrushFromSoftTexture(_Image);
}


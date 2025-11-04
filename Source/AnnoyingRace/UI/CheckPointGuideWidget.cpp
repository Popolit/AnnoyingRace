#include "CheckPointGuideWidget.h"

#include "RacePlayerState.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "World/CheckPointBox.h"

void UCheckPointGuideWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	OwnerController_ = GetOwningPlayer();
    PlayerState_ = OwnerController_->GetPlayerState<ARacePlayerState>();
}

void UCheckPointGuideWidget::NativeTick(const FGeometry& _MyGeometry, float _InDeltaTime)
{
	Super::NativeTick(_MyGeometry , _InDeltaTime);

    if (nullptr == OwnerController_ || nullptr == PlayerState_)
    {
	    return;
    }
	
    auto CurrentCheckpoint = Cast<ACheckPointBox>(PlayerState_->GetPassedCheckPoint());
    if (nullptr == CurrentCheckpoint)
    {
	    return;
    }

	//목표까지 거리 계산
	APawn* Pawn = OwnerController_->GetPawn();
	FVector PlayerLocation = CurrentCheckpoint->GetActorLocation();
	if (Pawn)
	{
		 PlayerLocation = Pawn->GetActorLocation();
	}
	
	FVector TargetLocation = CurrentCheckpoint->GetTargetCheckPoint()->GetActorLocation();
    double Distance = FVector::Distance(PlayerLocation, TargetLocation) / 100.f;
    FString DistanceString = FString::Printf(TEXT("%d m"), static_cast<int32>(Distance));
    Txt_Distance_->SetText(FText::FromString(DistanceString));
	
    FVector2D ScreenPosition;
	bool bIsVisible = UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(OwnerController_, TargetLocation, ScreenPosition, false);

	//가이드 마커 표기
	if (bIsVisible)
	{
		Img_GuideMarker_->SetVisibility(ESlateVisibility::HitTestInvisible);
		Txt_Distance_->SetVisibility(ESlateVisibility::HitTestInvisible);

		const FVector2D ScreenSize = _MyGeometry.GetLocalSize();
		ScreenPosition.X = FMath::Clamp(ScreenPosition.X, Padding_, ScreenSize.X - Padding_);
		ScreenPosition.Y = FMath::Clamp(ScreenPosition.Y, Padding_, ScreenSize.Y - Padding_);
		UCanvasPanelSlot* MarkerSlot = Cast<UCanvasPanelSlot>(Img_GuideMarker_->Slot);
		UCanvasPanelSlot* TextSlot = Cast<UCanvasPanelSlot>(Txt_Distance_->Slot);
		MarkerSlot->SetPosition(ScreenPosition);
		TextSlot->SetPosition(ScreenPosition);
	}
	else
	{
		Img_GuideMarker_->SetVisibility(ESlateVisibility::Collapsed);
		Txt_Distance_->SetVisibility(ESlateVisibility::Collapsed);
	}
}

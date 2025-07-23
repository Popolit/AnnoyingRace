#include "PlayerHUDWidget.h"

#include "RaceGameState.h"
#include "RacePlayerState.h"
#include "Components/TextBlock.h"

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ARacePlayerState* PS = GetOwningPlayerState<ARacePlayerState>();
	check(PS);

	ARaceGameState* GS = GetWorld()->GetGameState<ARaceGameState>();
	check(GS);

	MaxLap_ = GS->GetMaxLap();

	PS->OnLapsChanged_.BindUObject(this, &UPlayerHUDWidget::UpdateUserLap);
	UpdateUserLap(PS->GetLaps());
}

void UPlayerHUDWidget::UpdateUserRank()
{
	//TODO : Rank 시스템 제작
}

void UPlayerHUDWidget::UpdateUserLap(uint8 _Laps)
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("CurrentLap"), FText::AsNumber(_Laps));
	Args.Add(TEXT("MaxLap"), FText::AsNumber(MaxLap_));
	Txt_UserLap_->SetText(FText::Format(FText::FromString(TEXT("{CurrentLap} / {MaxLap}")), Args));
}

#include "PlayerHUDWidget.h"

#include "RaceGameState.h"
#include "RacePlayerState.h"
#include "RaceRankSlotWidget.h"
#include "Characters/CharacterData.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ARaceGameState* GS = GetWorld()->GetGameState<ARaceGameState>();
	check(GS);

	GS->OnPlayerRankingUpdated_.BindUObject(this, &UPlayerHUDWidget::UpdateUserRank);

	//Init LeaderBoard
	if (ensureMsgf(CP_LeaderBoard_, TEXT("PlayerList Canvas was null")))
	{
		CP_LeaderBoard_->ClearChildren();

		const TArray<APlayerState*>& Rankings = GS->GetPlayerRankings();
		URaceRankSlotWidget* SlotWidget;
		for (uint8 Rank = 0; Rank < Rankings.Num(); Rank++)
		{
			APlayerState* PlayerState = Rankings[Rank];
			ARacePlayerState* RacePlayerState = Cast<ARacePlayerState>(PlayerState);
			if (ensure(RacePlayerState && SlotWidgetClass_))
			{
				SlotWidget = CreateWidget<URaceRankSlotWidget>(this, SlotWidgetClass_);
				UCharacterData* CharacterData = RacePlayerState->GetCharacterData();
				RacePlayerState->OnCharacterDataSet_.BindUObject(SlotWidget, &URaceRankSlotWidget::SetSlotFromCharacterData);
				if(CharacterData)
				{
					SlotWidget->SetSlotFromCharacterData(CharacterData);
				}
				CP_LeaderBoard_->AddChildToCanvas(SlotWidget);
				PlayerSlots_.Add(PlayerState, SlotWidget);
				SlotWidget->SetRank(Rank);

				if (PlayerState == GetOwningPlayerState())
				{
					FFormatNamedArguments Args;
					Args.Add(TEXT("UserRank"), FText::AsNumber(Rank + 1));
					Args.Add(TEXT("UserCount"), FText::AsNumber(Rankings.Num()));
					Txt_UserRank_->SetText(FText::Format(FText::FromString(TEXT("{UserRank} / {UserCount}")), Args));

				}
			}
		}
	}

	MaxLap_ = GS->GetMaxLap();

	ARacePlayerState* PS = GetOwningPlayerState<ARacePlayerState>();
	check(PS);

	PS->OnLapsChanged_.BindUObject(this, &UPlayerHUDWidget::UpdateUserLap);
	UpdateUserLap(PS->GetLaps());
}

void UPlayerHUDWidget::UpdateUserRank() const
{
	auto GS = Cast<ARaceGameState>(UGameplayStatics::GetGameState(this));
	check(GS);

	const TArray<TObjectPtr<APlayerState>> Rankings = GS->GetPlayerRankings();

	for(uint8 Rank = 0; Rank < Rankings.Num(); Rank++)
	{
		APlayerState* PS = Rankings[Rank];

		if(PS == GetOwningPlayerState())
		{
			FFormatNamedArguments Args;
			Args.Add(TEXT("UserRank"), FText::AsNumber(Rank + 1));
			Args.Add(TEXT("UserCount"), FText::AsNumber(Rankings.Num()));
			Txt_UserRank_->SetText(FText::Format(FText::FromString(TEXT("{UserRank} / {UserCount}")), Args));
		}
		URaceRankSlotWidget* RankSlot = PlayerSlots_.FindRef(PS);
		if (ensureMsgf(RankSlot, TEXT("RankSlot from PlayerSlots was nullptr")))
		{
			RankSlot->SetRank(Rank);
		}
	}
}

void UPlayerHUDWidget::UpdateUserLap(uint8 _Laps) const
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("CurrentLap"), FText::AsNumber(_Laps));
	Args.Add(TEXT("MaxLap"), FText::AsNumber(MaxLap_));
	Txt_UserLap_->SetText(FText::Format(FText::FromString(TEXT("{CurrentLap} / {MaxLap}")), Args));
}

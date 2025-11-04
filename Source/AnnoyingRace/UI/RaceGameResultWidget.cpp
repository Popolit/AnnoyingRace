#include "RaceGameResultWidget.h"

#include "RaceGameResultSlotWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"

void URaceGameResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Back_->OnClicked.AddDynamic(this, &URaceGameResultWidget::OnBackButtonClicked);
}

void URaceGameResultWidget::SetGameResult(const TArray<FRaceGameResultData>& _PrevRaceGameResult)
{
	//플레이어 랭킹 슬롯 작성
	if (ensureMsgf(SlotWidgetClass_, TEXT("Race Game Result Slot Widget Class was not set")))
	{
		VTB_GameResult_->ClearChildren();
		uint8 NumOfPlayers = _PrevRaceGameResult.Num();
		for (uint8 i = 0; i < NumOfPlayers; i++)
		{
			URaceGameResultSlotWidget* Child = CreateWidget<URaceGameResultSlotWidget>(this, SlotWidgetClass_);
			if (Child)
			{
				VTB_GameResult_->AddChildToVerticalBox(Child);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to Create Race Game Result Slot Widget"));
			}
		}
		for (const auto& Elem : _PrevRaceGameResult)
		{
			auto Child = Cast<URaceGameResultSlotWidget>(VTB_GameResult_->GetChildAt(Elem.PlayerRank_ - 1));
			if (Child)
			{
				Child->SetWidget(Elem.PlayerRank_, Elem.PlayerName_);
			}
		}
	}
}

void URaceGameResultWidget::OnBackButtonClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

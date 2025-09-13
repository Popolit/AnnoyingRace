#include "WaitingPlayersWidget.h"

#include "RaceGameInstance.h"
#include "Components/TextBlock.h"

void UWaitingPlayersWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto GI = GetGameInstance<URaceGameInstance>();
	check( GI );

	Txt_TotalCount_->SetText(FText::AsNumber(GI->GetRacePlayerCount()));
}

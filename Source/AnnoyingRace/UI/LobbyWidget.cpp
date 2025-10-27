#include "LobbyWidget.h"

#include "LobbyPlayerController.h"
#include "Components/Button.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);
	
	auto PC = Cast<ALobbyPlayerController>(GetOwningPlayer());
	check(PC);
	
	Btn_CreateRoom_->OnClicked.AddDynamic(PC, &ALobbyPlayerController::OpenCreateSession);
	Btn_JoinGame_->OnClicked.AddDynamic(PC, &ALobbyPlayerController::OpenJoinSession);
	Btn_Option_->OnClicked.AddDynamic(PC, &ALobbyPlayerController::OpenOption);
	Btn_Exit_->OnClicked.AddDynamic(PC, &ALobbyPlayerController::OpenExitDialogue);
}

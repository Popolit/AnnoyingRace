#include "ConfirmExitGameWidget.h"

#include "RacePlayerController.h"
#include "Components/Button.h"

UConfirmExitGameWidget::UConfirmExitGameWidget(const FObjectInitializer& _ObjectInitializer)
	:Super(_ObjectInitializer)
{
	SetIsFocusable(true);
}

void UConfirmExitGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto* PC = GetOwningPlayer<ARacePlayerController>();

	Btn_Yes_->OnClicked.AddDynamic(PC, &ARacePlayerController::ExitGame);
	Btn_No_->OnClicked.AddDynamic(PC, &ARacePlayerController::CloseConfirmExitGameDialog);
}

#include "RaceMenuWidget.h"

#include "RacePlayerController.h"
#include "OptionWidget.h"
#include "Components/Button.h"

URaceMenuWidget::URaceMenuWidget(const FObjectInitializer& _ObjectInitializer)
	: Super(_ObjectInitializer)
{
	SetIsFocusable(true);
}

void URaceMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ARacePlayerController* PC = GetOwningPlayer<ARacePlayerController>();
	check(PC);

	Btn_Resume_->OnClicked.AddDynamic(PC, &ARacePlayerController::CloseMainMenu);
	Btn_Option_->OnClicked.AddDynamic(PC, &ARacePlayerController::OpenOptionMenu);
	Btn_Exit_->OnClicked.AddDynamic(PC, &ARacePlayerController::OpenConfirmExitGameDialog);
}

FReply URaceMenuWidget::NativeOnKeyDown(const FGeometry& _InGeometry, const FKeyEvent& _InKeyEvent)
{
	//ESC
	if (_InKeyEvent.GetKey() == EKeys::Escape)
	{
		if (auto* PC = GetOwningPlayer<ARacePlayerController>())
		{
			PC->CloseMainMenu();
		}
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(_InGeometry, _InKeyEvent);
}
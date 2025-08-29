#include "ExitDialogueWidget.h"

#include "Components/Button.h"

UExitDialogueWidget::UExitDialogueWidget(const FObjectInitializer& _ObjectInitializer)
	:Super(_ObjectInitializer)
{
	SetIsFocusable(true);
}

void UExitDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Yes_->OnClicked.AddDynamic(this, &UExitDialogueWidget::OnClckedYesBtn_);
	Btn_No_->OnClicked.AddDynamic(this, &UExitDialogueWidget::OnClckedNoBtn_);
}

void UExitDialogueWidget::OnClckedYesBtn_()
{
	OnYesBtnClicked_.ExecuteIfBound();
}

void UExitDialogueWidget::OnClckedNoBtn_()
{
	OnNoBtnClicked_.ExecuteIfBound();
}

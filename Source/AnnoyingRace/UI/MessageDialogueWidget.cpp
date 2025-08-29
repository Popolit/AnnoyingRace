#include "MessageDialogueWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"



UMessageDialogueWidget::UMessageDialogueWidget(const FObjectInitializer& _ObjectInitializer)
	: Super(_ObjectInitializer)
{
	SetIsFocusable(true);
}

void UMessageDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Close_->OnClicked.AddDynamic(this, &UMessageDialogueWidget::OnClickedClose);
}

FReply UMessageDialogueWidget::NativeOnKeyDown(const FGeometry& _InGeometry, const FKeyEvent& _InKeyEvent)
{
	if (_InKeyEvent.GetKey() == EKeys::Escape)
	{
		OnClickedClose_.ExecuteIfBound();
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(_InGeometry, _InKeyEvent);
}

void UMessageDialogueWidget::SetMessage(const FText& _Message) const
{
	Txt_Message_->SetText(_Message);
}

void UMessageDialogueWidget::OnClickedClose()
{
	OnClickedClose_.ExecuteIfBound();
}
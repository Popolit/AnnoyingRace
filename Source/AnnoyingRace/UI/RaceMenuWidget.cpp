#include "RaceMenuWidget.h"

#include "Components/Button.h"

URaceMenuWidget::URaceMenuWidget(const FObjectInitializer& _ObjectInitializer)
	: Super(_ObjectInitializer)
{
	bIsFocusable = true;
}

void URaceMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	APlayerController* PC = GetOwningPlayer();
	if(ensure(PC))
	{
		PC->bShowMouseCursor = true;

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());
		PC->SetInputMode(InputMode);
	}

	Btn_Resume_->OnClicked.AddDynamic(this, &URaceMenuWidget::ResumeGame);
	OnNativeVisibilityChanged.AddUObject(this, &URaceMenuWidget::VisibilityChanged);
}

FReply URaceMenuWidget::NativeOnKeyDown(const FGeometry& _InGeometry, const FKeyEvent& _InKeyEvent)
{
	//ESC
	if (_InKeyEvent.GetKey() == EKeys::Escape)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(_InGeometry, _InKeyEvent);
}

void URaceMenuWidget::ResumeGame()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void URaceMenuWidget::VisibilityChanged(ESlateVisibility _Visibility)
{
	APlayerController* PC = GetOwningPlayer();
	if(nullptr == PC)
	{
		ensure(false);
		return; 
	}

	if(_Visibility == ESlateVisibility::Visible)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}
	else
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
	}
}

#include "OptionWidget.h"

#include "RacePlayerController.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

UOptionWidget::UOptionWidget(const FObjectInitializer& _ObjectInitializer)
	: Super(_ObjectInitializer)
{
	SetIsFocusable(true);
}

void UOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OptionButtons_.Add(Btn_Graphics_);
	OptionButtons_.Add(Btn_Audio_);
	OptionButtons_.Add(Btn_Language_);
	OptionButtons_.Add(Btn_Control_);
	SelectedNum_ = 0;
	WST_Options_->SetActiveWidgetIndex(SelectedNum_);

	ARacePlayerController* PC = GetOwningPlayer<ARacePlayerController>();
	check(PC);

	Btn_Graphics_->OnClicked.AddDynamic(this, &UOptionWidget::OnClickedGraphicsBtn);
	Btn_Audio_->OnClicked.AddDynamic(this, &UOptionWidget::OnClickedAudioBtn);
	Btn_Language_->OnClicked.AddDynamic(this, &UOptionWidget::OnClickedLanguageBtn);
	Btn_Control_->OnClicked.AddDynamic(this, &UOptionWidget::OnClickedControlBtn);
	Btn_Back_->OnClicked.AddDynamic(PC, &ARacePlayerController::CloseOptionMenu);
}

void UOptionWidget::OnClickedGraphicsBtn()
{
	OnOptionSelected(0);
}

void UOptionWidget::OnClickedAudioBtn()
{
	OnOptionSelected(1);
}

void UOptionWidget::OnClickedLanguageBtn()
{
	OnOptionSelected(2);
}

void UOptionWidget::OnClickedControlBtn()
{
	OnOptionSelected(3);
}

void UOptionWidget::OnOptionSelected(uint8 _SelectedNum)
{
	if(false == OptionButtons_.IsValidIndex(SelectedNum_) || false == OptionButtons_.IsValidIndex(_SelectedNum))
	{
		ensureMsgf(false, TEXT("Option Btn Array Index was not valid"));
		return;
	}
	
	//기존에 선택됐던 버튼
	OptionButtons_[SelectedNum_]->SetColorAndOpacity(FLinearColor(255, 255, 255, 1.0));

	SelectedNum_ = _SelectedNum;
	OptionButtons_[SelectedNum_]->SetColorAndOpacity(FLinearColor(0, 0, 0, 0.5));

	WST_Options_->SetActiveWidgetIndex(SelectedNum_);
}


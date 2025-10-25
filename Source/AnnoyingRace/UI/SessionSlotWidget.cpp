#include "SessionSlotWidget.h"

#include "RaceGameInstance.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void USessionSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Enter_->OnClicked.AddDynamic(this, &USessionSlotWidget::OnClickedEnter);
}

void USessionSlotWidget::SetSessionData(UCommonSession_SearchResult* _SessionSearchResult)
{
	SessionSearchResult_ = _SessionSearchResult;
}

void USessionSlotWidget::SetSessionName(const FString& _SessionName)
{
	Txt_SessionName_->SetText(FText::FromString(_SessionName));
}

void USessionSlotWidget::SetUserCount(int32 _CurrUserCount, int32 _MaxUserCount)
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("Curr"), FText::AsNumber(_CurrUserCount));
	Args.Add(TEXT("Max"), FText::AsNumber(_MaxUserCount));
	Txt_UserCount_->SetText(FText::Format(FText::FromString(TEXT("{Curr} / {Max}")), Args));
}

void USessionSlotWidget::SetIsPrivateSession(bool _bPrivate)
{
	if (_bPrivate)
	{
		Img_Lock_->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	//Public Session
	else
	{
		Img_Lock_->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USessionSlotWidget::SetMapThumbnail(const TSoftObjectPtr<UTexture2D>& _MapThumbnail)
{
	Img_Map_->SetBrushFromSoftTexture(_MapThumbnail);
}

//Session에 참가
void USessionSlotWidget::OnClickedEnter()
{
	if (false == SessionSearchResult_.IsValid())
	{
		ensureMsgf(false, TEXT("SessionSearchResult was null"));
		return;
	}
	URaceGameInstance* GI = GetGameInstance<URaceGameInstance>();
	if (GI)
	{
		GI->JoinSessionWithResult(GetOwningPlayer(), SessionSearchResult_.Get());
	}
}
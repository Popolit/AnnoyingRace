#include "SessionSlotWidget.h"

#include "CommonSessionSubsystem.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


void USessionSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Enter_->OnClicked.AddDynamic(this, &USessionSlotWidget::OnClickedEnter);
}

void USessionSlotWidget::SetSessionData(UCommonSession_SearchResult* _SessionSearchResult)
{
	if (nullptr == _SessionSearchResult)
	{
		return;
	}
	
	SessionSearchResult_ = _SessionSearchResult;
	
	//Session Name
	FString SessionName;
	bool IsSettingExist;
	_SessionSearchResult->GetStringSetting(TEXT("SESSION_NAME"), SessionName, IsSettingExist);
	if (ensureMsgf(IsSettingExist, TEXT("Session Name Setting was not found")))
	{
		Txt_SessionName_->SetText(FText::FromString(SessionName));
	}

	//Get Private Password
	_SessionSearchResult->GetStringSetting(TEXT("PASSWORD"), SessionPassword_, IsSettingExist);
	
	//Private Session
	if (IsSettingExist)
	{
		Img_Lock_->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	//Public Session
	else
	{
		Img_Lock_->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	FFormatNamedArguments Args;
	int32 MaxUserCount = _SessionSearchResult->GetMaxPublicConnections();
	int32 CurrUserCount = MaxUserCount - _SessionSearchResult->GetNumOpenPublicConnections();
	Args.Add(TEXT("Curr"), FText::AsNumber(CurrUserCount));
	Args.Add(TEXT("Max"), FText::AsNumber(MaxUserCount));
	Txt_UserCount_->SetText(FText::Format(FText::FromString(TEXT("{Curr} / {Max}")), Args));
}

//Session에 참가
void USessionSlotWidget::OnClickedEnter()
{
	if (false == SessionSearchResult_.IsValid())
	{
		ensureMsgf(false, TEXT("SessionSearchResult was null"));
		return;
	}
	UGameInstance* GI = UGameplayStatics::GetGameInstance(this);
	if (GI)
	{
		UCommonSessionSubsystem* SessionSubsystem = GI->GetSubsystem<UCommonSessionSubsystem>();
		APlayerController* PC = GetOwningPlayer();

		if (ensure(SessionSubsystem && PC))
		{
			if (false == SessionPassword_.IsEmpty())
			{
				//비밀번호 Dialogue
			}
			SessionSubsystem->JoinSession(PC, SessionSearchResult_.Get());
		}
	}
}
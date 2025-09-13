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
	bool IsSettingExist;
	_SessionSearchResult->GetStringSetting(TEXT("SESSION_NAME"), SessionName_, IsSettingExist);
	if (ensureMsgf(IsSettingExist, TEXT("Session Name Setting was not found")))
	{
		Txt_SessionName_->SetText(FText::FromString(SessionName_));
	}

	//Get Private Password
	_SessionSearchResult->GetStringSetting(TEXT("PASSWORD"), SessionPassword_, IsSettingExist);

	bSessionLocked_ = IsSettingExist;
	//Private Session
	if (bSessionLocked_)
	{
		Img_Lock_->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	//Public Session
	else
	{
		Img_Lock_->SetVisibility(ESlateVisibility::Collapsed);
	}

	FString MapDataString;
	_SessionSearchResult->GetStringSetting(TEXT("MAPDATA"), MapDataString, IsSettingExist);

	if(IsSettingExist)
	{
		MapData_ = FPrimaryAssetId(MapDataString);
	}
	FFormatNamedArguments Args;
	MaxUserCount_ = _SessionSearchResult->GetMaxPublicConnections();
	int32 CurrUserCount = MaxUserCount_ - _SessionSearchResult->GetNumOpenPublicConnections();
	Args.Add(TEXT("Curr"), FText::AsNumber(CurrUserCount));
	Args.Add(TEXT("Max"), FText::AsNumber(MaxUserCount_));
	Txt_UserCount_->SetText(FText::Format(FText::FromString(TEXT("{Curr} / {Max}")), Args));
}

FString USessionSlotWidget::GetSessionName()
{
	return SessionName_;
}

int32 USessionSlotWidget::GetMaxUserCount() const
{
	return MaxUserCount_;
}

bool USessionSlotWidget::IsSessionLocked() const
{
	return bSessionLocked_;
}

FPrimaryAssetId USessionSlotWidget::GetMapData() const
{
	return MapData_;
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
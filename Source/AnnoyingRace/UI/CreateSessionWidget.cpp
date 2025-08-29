#include "CreateSessionWidget.h"

#include "LobbyPlayerController.h"
#include "RaceGameInstance.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Engine/AssetManager.h"
#include "World/MapData.h"

#define LOCTEXT_NAMESPACE "ErrorMessages"

void UCreateSessionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	LastValidUserCount_ = ETB_MaxUserCount_->GetText();
	CHB_IsPrivate_->OnCheckStateChanged.AddDynamic(this, &UCreateSessionWidget::OnCheckPrivateStateChanged);
	ETB_MaxUserCount_->OnTextChanged.AddDynamic(this, &UCreateSessionWidget::OnMaxUserCountChanged);
	ETB_MaxUserCount_->OnTextCommitted.AddDynamic(this, &UCreateSessionWidget::OnMaxUserCountCommitted);
	
	auto PC = Cast<ALobbyPlayerController>(GetOwningPlayer());
	if (ensureMsgf(PC, TEXT("PlayerController was nullptr")))
	{
		Btn_Cancel_->OnClicked.AddDynamic(PC, &ALobbyPlayerController::CloseCreateSession);
		Btn_Create_->OnClicked.AddDynamic(this, &UCreateSessionWidget::OnClickedCreateBtn);
	}

	//맵 리스트 초기화
	auto& AssetManager = UAssetManager::Get();
	TArray<FPrimaryAssetId> MapAssetIds;
	AssetManager.GetPrimaryAssetIdList("MapData", MapAssetIds);
	AssetManager.LoadPrimaryAssets(MapAssetIds, TArray<FName>(),
		FStreamableDelegate::CreateUObject(this, &UCreateSessionWidget::OnMapsLoaded));
}

void UCreateSessionWidget::OnCheckPrivateStateChanged(bool _bChecked)
{
	ETB_Password_->SetIsReadOnly(!_bChecked);
}

void UCreateSessionWidget::OnMaxUserCountChanged(const FText& _Text)
{
	if (_Text.IsEmpty())
	{
		return;
	}
	
	if (false == _Text.IsNumeric())
	{
		ETB_MaxUserCount_->SetText(LastValidUserCount_);
		return;
	}

	int32 Num = FCString::Atoi(*_Text.ToString());
	if (Num <= 0)
	{
		LastValidUserCount_ = FText::AsNumber(1);
		ETB_MaxUserCount_->SetText(LastValidUserCount_);
	}
	//차후 MaxUserNumber에 대한 세팅이 있을 경우 변경
	else if (8 < Num)
	{
		LastValidUserCount_ = FText::AsNumber(8);
		ETB_MaxUserCount_->SetText(LastValidUserCount_);
	}
	//이상 없음
	else
	{
		LastValidUserCount_ = _Text;
	}
}

void UCreateSessionWidget::OnMaxUserCountCommitted(const FText& _Text, ETextCommit::Type _CommitType)
{
	if (_Text.IsEmpty())
	{
		ETB_MaxUserCount_->SetText(LastValidUserCount_);
		return;
	}

	if (false == _Text.IsNumeric())
	{
		ETB_MaxUserCount_->SetText(LastValidUserCount_);
		return;
	}

	int32 Num = FCString::Atoi(*_Text.ToString());
	if (Num <= 0)
	{
		LastValidUserCount_ = FText::AsNumber(1);
		ETB_MaxUserCount_->SetText(LastValidUserCount_);
	}
	//차후 MaxUserNumber에 대한 세팅이 있을 경우 변경
	else if (8 < Num)
	{
		LastValidUserCount_ = FText::AsNumber(8);
		ETB_MaxUserCount_->SetText(LastValidUserCount_);
	}
	//이상 없음
	else
	{
		LastValidUserCount_ = _Text;
	}
}

//모든 요소를 검사하고 세션을 생성함
void UCreateSessionWidget::OnClickedCreateBtn()
{
	FText ErrorMessage;
	if (CheckAllPropertyIsValid(ErrorMessage))
	{
		URaceGameInstance* GI = GetGameInstance<URaceGameInstance>();
		if (ensureMsgf(GI, TEXT("Game Instance class was not set")))
		{
			APlayerController* PC = GetOwningPlayer();
			FSessionData SessionData;
			
			SessionData.SessionName_ = ETB_SessionName_->GetText().ToString();
			SessionData.bPrivate_ = CHB_IsPrivate_->IsChecked();
			if (CHB_IsPrivate_->IsChecked())
			{
				SessionData.Password_ = ETB_Password_->GetText().ToString();
			}
			SessionData.MaxUserCount_ = FCString::Atoi(*ETB_MaxUserCount_->GetText().ToString());
			
			auto& AssetManager = UAssetManager::Get();
			TArray<FPrimaryAssetId> MapAssetIds;
			AssetManager.GetPrimaryAssetIdList("MapData", MapAssetIds);

			//맵 리스트 순회, 고른 맵을 찾아 세팅
			for (const auto& AssetId : MapAssetIds)
			{
				auto MapData = Cast<UMapData>(AssetManager.GetPrimaryAssetObject(AssetId));
				if (MapData && MapData->MapDisplayName_.ToString() == CBB_MapList_->GetSelectedOption())
				{
					SessionData.MapData_ = AssetId;
				}
			}
			GI->CreateSession(PC, SessionData);
		}
	}
	else
	{
		auto PC = GetOwningPlayer<ALobbyPlayerController>();
		if (PC)
		{
			PC->OpenMessageDialogue(ErrorMessage);
		}
	}
}

//비동기 애셋 로드, 콤보 박스 요소 채우기
void UCreateSessionWidget::OnMapsLoaded()
{
	auto& AssetManager = UAssetManager::Get();
	TArray<FPrimaryAssetId> MapAssetIds;
	AssetManager.GetPrimaryAssetIdList(TEXT("MapData"), MapAssetIds);
    
	for (const auto& AssetId : MapAssetIds)
	{
		auto MapData = Cast<UMapData>(AssetManager.GetPrimaryAssetObject(AssetId));
    
		if (MapData)
		{
			CBB_MapList_->AddOption(MapData->MapDisplayName_.ToString());
		}
	}
	CBB_MapList_->SetSelectedIndex(0);
}


bool UCreateSessionWidget::CheckAllPropertyIsValid(FText& _ErrorMessage)
{
	if (ETB_SessionName_->GetText().IsEmpty())
	{
		_ErrorMessage = LOCTEXT("Error_EmptySessionName", "Session Name cannot be empty.");
		return false;
	}
	if (CHB_IsPrivate_->IsChecked() && ETB_Password_->GetText().IsEmpty())
	{
		_ErrorMessage = LOCTEXT("Error_EmptyPassword", "Private session's Password cannot be empty.");
		return false;
	}
	const FText& Text = ETB_MaxUserCount_->GetText();
	if (false == Text.IsNumeric())
	{
		_ErrorMessage = LOCTEXT("Error_InvalidUserCount", "Player count must be between 1 and 8.");
		return false;
	}
	int32 Num = FCString::Atoi(*Text.ToString());
	if (Num <= 0 || 8 < Num)
	{
		_ErrorMessage = LOCTEXT("Error_InvalidUserCount", "Player count must be between 1 and 8.");
		return false;
	}
	if (CBB_MapList_->GetSelectedOption().IsEmpty())
	{
		_ErrorMessage = LOCTEXT("Error_InvalidMapList", "Map list cannot be empty.");
		return false;
	}
	
	return true;
}
#undef LOCTEXT_NAMESPACE
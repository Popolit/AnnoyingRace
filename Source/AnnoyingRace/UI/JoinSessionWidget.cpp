#include "JoinSessionWidget.h"

#include "CommonSessionSubsystem.h"
#include "LobbyPlayerController.h"
#include "RaceGameInstance.h"
#include "SessionSlotWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Engine/AssetManager.h"
#include "World/MapData.h"

#define LOCTEXT_NAMESPACE "ErrorMessages"

void UJoinSessionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SCB_SessionList_->ClearChildren();
	CBB_MapList_->OnSelectionChanged.AddDynamic(this, &UJoinSessionWidget::OnMapChanged);
	ETB_SessionName_->OnTextChanged.AddDynamic(this, &UJoinSessionWidget::OnSessionNameChanged);
	ETB_SessionName_->OnTextCommitted.AddDynamic(this, &UJoinSessionWidget::OnSessionNameCommitted);
	ETB_MinUserCount_->OnTextChanged.AddDynamic(this, &UJoinSessionWidget::OnMinUserChanged);
	ETB_MinUserCount_->OnTextCommitted.AddDynamic(this, &UJoinSessionWidget::OnMinUserCommitted);
	ETB_MaxUserCount_->OnTextChanged.AddDynamic(this, &UJoinSessionWidget::OnMaxUserChanged);
	ETB_MinUserCount_->OnTextCommitted.AddDynamic(this, &UJoinSessionWidget::OnMaxUserCommitted);
	Btn_Refresh_->OnClicked.AddDynamic(this, &UJoinSessionWidget::OnClickRefresh);

	auto PC = Cast<ALobbyPlayerController>(GetOwningPlayer());
	if (ensureMsgf(PC, TEXT("Lobby Player Controller was nullptr")))
	{
		Btn_Back_->OnClicked.AddDynamic(PC, &ALobbyPlayerController::CloseJoinSession);
		URaceGameInstance* GI = GetGameInstance<URaceGameInstance>();
		if (ensureMsgf(GI, TEXT("Game Instance class was not set")))
		{
			GI->OnSessionFindComplete_.BindUObject(this, &UJoinSessionWidget::OnSessionFindFinished);
			GI->FindSessions(PC);
		}
	}

	CBB_MapList_->ClearOptions();
	CBB_MapList_->AddOption( "Any" );
	CBB_MapList_->SetSelectedOption( "Any" );

	//Map 리스트 로드
	auto& AssetManager = UAssetManager::Get();
	TArray<FPrimaryAssetId> MapAssetDatas;
	AssetManager.GetPrimaryAssetIdList( "MapData" , MapAssetDatas );
	AssetManager.LoadPrimaryAssets( MapAssetDatas , TArray<FName>() ,
		FStreamableDelegate::CreateUObject( this , &UJoinSessionWidget::OnMapsLoaded ));
}

void UJoinSessionWidget::OnMapChanged(FString _MapName, ESelectInfo::Type _Type)
{
	FPrimaryAssetId MapAssetId = FindMapAssetIdByMapName(_MapName);
	if (MapAssetId.IsValid())
	{
		auto& AssetManager = UAssetManager::Get();
		UMapData* MapData = Cast<UMapData>(AssetManager.GetPrimaryAssetObject(MapAssetId));

		Img_Map_->SetBrushFromSoftTexture(MapData->MapThumbnail_);
	}
	UpdateSessionList();
}

void UJoinSessionWidget::OnSessionNameChanged(const FText& _Text)
{
	UpdateSessionList();
}

void UJoinSessionWidget::OnSessionNameCommitted(const FText& _Text, ETextCommit::Type _CommitMethod)
{
	if (_CommitMethod != ETextCommit::Type::OnEnter)
	{
		return;
	}

	UpdateSessionList();
}

void UJoinSessionWidget::OnMinUserChanged(const FText& _Text)
{
	if (_Text.IsEmpty())
	{
		return;
	}
	
	if (false == _Text.IsNumeric())
	{
		ETB_MinUserCount_->SetText(LastValidMinUserCount_);
		return;
	}

	int32 Num = FCString::Atoi(*_Text.ToString());
	if (Num <= 0)
	{
		LastValidMinUserCount_ = FText::AsNumber(1);
		ETB_MinUserCount_->SetText(LastValidMinUserCount_);
	}
	//차후 MaxUserNumber에 대한 세팅이 있을 경우 변경
	else if (8 < Num)
	{
		LastValidMinUserCount_ = FText::AsNumber(8);
		ETB_MinUserCount_->SetText(LastValidMinUserCount_);
	}
	//이상 없음
	else
	{
		LastValidMinUserCount_ = _Text;
		UpdateSessionList();
	}
}

void UJoinSessionWidget::OnMinUserCommitted(const FText& _Text, ETextCommit::Type _CommitMethod)
{
	if (_Text.IsEmpty())
	{
		ETB_MinUserCount_->SetText(LastValidMinUserCount_);
		return;
	}

	if (false == _Text.IsNumeric())
	{
		ETB_MinUserCount_->SetText(LastValidMinUserCount_);
		return;
	}

	int32 Num = FCString::Atoi(*_Text.ToString());
	if (Num <= 0)
	{
		LastValidMinUserCount_ = FText::AsNumber(1);
		ETB_MinUserCount_->SetText(LastValidMinUserCount_);
	}
	//차후 MaxUserNumber에 대한 세팅이 있을 경우 변경
	else if (8 < Num)
	{
		LastValidMinUserCount_ = FText::AsNumber(8);
		ETB_MinUserCount_->SetText(LastValidMinUserCount_);
	}
	//이상 없음
	else
	{
		LastValidMinUserCount_ = _Text;
		UpdateSessionList();
	}
}

void UJoinSessionWidget::OnMaxUserChanged(const FText& _Text)
{
	if (_Text.IsEmpty())
	{
		return;
	}
	
	if (false == _Text.IsNumeric())
	{
		ETB_MaxUserCount_->SetText(LastValidMaxUserCount_);
		return;
	}

	int32 Num = FCString::Atoi(*_Text.ToString());
	if (Num <= 0)
	{
		LastValidMaxUserCount_ = FText::AsNumber(1);
		ETB_MaxUserCount_->SetText(LastValidMaxUserCount_);
	}
	//차후 MaxUserNumber에 대한 세팅이 있을 경우 변경
	else if (8 < Num)
	{
		LastValidMaxUserCount_ = FText::AsNumber(8);
		ETB_MaxUserCount_->SetText(LastValidMaxUserCount_);
	}
	//이상 없음
	else
	{
		LastValidMaxUserCount_ = _Text;
		UpdateSessionList();
	}
}

void UJoinSessionWidget::OnMaxUserCommitted(const FText& _Text, ETextCommit::Type _CommitMethod)
{
	if (_Text.IsEmpty())
	{
		ETB_MaxUserCount_->SetText(LastValidMaxUserCount_);
		return;
	}

	if (false == _Text.IsNumeric())
	{
		ETB_MaxUserCount_->SetText(LastValidMaxUserCount_);
		return;
	}

	int32 Num = FCString::Atoi(*_Text.ToString());
	if (Num <= 0)
	{
		LastValidMaxUserCount_ = FText::AsNumber(1);
		ETB_MaxUserCount_->SetText(LastValidMaxUserCount_);
	}
	//차후 MaxUserNumber에 대한 세팅이 있을 경우 변경
	else if (8 < Num)
	{
		LastValidMaxUserCount_ = FText::AsNumber(8);
		ETB_MaxUserCount_->SetText(LastValidMaxUserCount_);
	}
	//이상 없음
	else
	{
		LastValidMaxUserCount_ = _Text;
		UpdateSessionList();
	}
}

void UJoinSessionWidget::OnClickRefresh()
{
	auto GI = GetGameInstance<URaceGameInstance>();
	if (GI)
	{
		GI->FindSessions(GetOwningPlayer());
	}
}

void UJoinSessionWidget::OnMapsLoaded()
{
	auto& AssetManager = UAssetManager::Get();
	TArray<FPrimaryAssetId> MapAssetDatas;
	AssetManager.GetPrimaryAssetIdList( "MapData" , MapAssetDatas );
	TArray<FString> TempMapNames;
	for ( const auto& AssetId : MapAssetDatas )
	{
		auto MapData = Cast<UMapData>( AssetManager.GetPrimaryAssetObject( AssetId ) );
		if ( ensureMsgf( MapData , TEXT( "Map Data was nullptr" ) ) )
		{
			//Random을 첫 요소로 고정
			if ( MapData->MapDisplayName_.ToString() == "Random" )
			{
				CBB_MapList_->AddOption( MapData->MapDisplayName_.ToString() );
			}
			else
			{
				TempMapNames.Add( MapData->MapDisplayName_.ToString() );
			}
		}
	}

	TempMapNames.Sort();
	//Random 제외한 맵 이름
	for ( const FString& MapName : TempMapNames )
	{
		CBB_MapList_->AddOption( MapName );
	}
}

void UJoinSessionWidget::OnSessionFindFinished(bool _bSuccess, const FText& _ErrorMessage, const TArray<UCommonSession_SearchResult*>& _Results)
{
	//ErrorDialogue
	if (false == _bSuccess)
	{
		auto PC = GetOwningPlayer<ALobbyPlayerController>();
		if (PC)
		{
			FText BaseMessage = LOCTEXT("Error_FindSessionFailed", "Failed to find session.");
			FFormatNamedArguments Args;
			Args.Add(TEXT("BaseError"), BaseMessage);
			Args.Add(TEXT("SystemError"), _ErrorMessage);

			FText FormattedMessage = FText::Format(LOCTEXT("CombinedErrorFormat", "{BaseError}\n{SystemError}"), Args);
			PC->OpenMessageDialogue(FormattedMessage);
		}
		return;
	}

	Results_ = _Results;
	UpdateSessionList();
}

//맵 리스트 순회, 찾는 맵 AssetID를 가져옴
FPrimaryAssetId UJoinSessionWidget::FindMapAssetIdByMapName(const FString& _MapName) const
{
	auto& AssetManager = UAssetManager::Get();
	TArray<FPrimaryAssetId> MapAssetIds;
	AssetManager.GetPrimaryAssetIdList("MapData", MapAssetIds);
	
	for (const auto& AssetId : MapAssetIds)
	{
		auto MapData = Cast<UMapData>(AssetManager.GetPrimaryAssetObject(AssetId));
		if (MapData && MapData->MapDisplayName_.ToString() == _MapName)
		{
			return AssetId;
		}
	}
	return FPrimaryAssetId();
}

void UJoinSessionWidget::UpdateSessionList()
{
	SCB_SessionList_->ClearChildren();

	//검색 결과에 맞춤
	for(const auto& Result : Results_)
	{
		bool IsSettingExist;
		FString SessionName;
		
		//Session Name
		Result->GetStringSetting(TEXT("SESSION_NAME"), SessionName, IsSettingExist);
		if (ensureMsgf(IsSettingExist, TEXT("Session Name Setting was not found")))
		{
			FString SearchingName = ETB_SessionName_->GetText().ToString();
			if(false == SessionName.Contains(SearchingName))
			{
				continue;
			}
		}

		FString MapSetting;
		TSoftObjectPtr<UTexture2D> MapThumbnail;
		
		//Map
		Result->GetStringSetting(TEXT("MAPDATA"), MapSetting, IsSettingExist);
		if (IsSettingExist)
		{
			FString SelectedMapName = CBB_MapList_->GetSelectedOption();
			FPrimaryAssetId MapData = FPrimaryAssetId(MapSetting);
			auto& AssetManager = UAssetManager::Get();
			const auto Map = Cast<UMapData>(AssetManager.GetPrimaryAssetObject( MapData ));
			if(ensure(Map) && SelectedMapName != "Any")
			{
				if(Map->MapDisplayName_.ToString() != SelectedMapName)
				{
					continue;
				}
			}
			MapThumbnail = Map->MapThumbnail_;
		}

		//Player Count
		int32 MinUserCount = FCString::Atoi(*ETB_MinUserCount_->GetText().ToString());
		int32 MaxUserCount = FCString::Atoi(*ETB_MaxUserCount_->GetText().ToString());
		int32 UserCount = Result->GetMaxPublicConnections();
		int32 CurrUserCount = UserCount - Result->GetNumOpenPublicConnections();

		if(UserCount < MinUserCount || MaxUserCount < UserCount)
		{
			continue;
		}

		FString Password;
		//Get Private Password
		Result->GetStringSetting(TEXT("PASSWORD"), Password, IsSettingExist);

		USessionSlotWidget* NewSlot = CreateWidget<USessionSlotWidget>(this, SessionSlotWidgetClass_);
		NewSlot->SetSessionData(Result);
		NewSlot->SetSessionName(SessionName);
		NewSlot->SetUserCount(CurrUserCount, UserCount);
		NewSlot->SetMapThumbnail(MapThumbnail);
		NewSlot->SetIsPrivateSession(IsSettingExist);
		SCB_SessionList_->AddChild(NewSlot);
	}
}
#undef LOCTEXT_NAMESPACE

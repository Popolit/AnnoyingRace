#include "JoinSessionWidget.h"

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
			GI->FindSessions(PC, FSessionFindData());
		}
	}

	auto& AssetManager = UAssetManager::Get();
	TArray<FPrimaryAssetId> MapAssetIds;
	AssetManager.GetPrimaryAssetIdList("MapData", MapAssetIds);
	
	//맵 리스트 순회
	for (const auto& AssetId : MapAssetIds)
	{
		auto MapData = Cast<UMapData>(AssetManager.GetPrimaryAssetObject(AssetId));

		//맵 종류 채우기
		if (MapData)
		{
			CBB_MapList_->AddOption(MapData->MapDisplayName_.ToString());
		}
	}
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
	}
}

void UJoinSessionWidget::OnClickRefresh()
{
	auto GI = GetGameInstance<URaceGameInstance>();
	if (GI)
	{
		FSessionFindData SessionFindData;
		SessionFindData.SessionName_ = ETB_SessionName_->GetText().ToString();
		SessionFindData.MaxUserCount_ = FCString::Atoi(*ETB_MaxUserCount_->GetText().ToString());
		SessionFindData.MinUserCount_ = FCString::Atoi(*ETB_MinUserCount_->GetText().ToString());

		FPrimaryAssetId MapAssetId = FindMapAssetIdByMapName(CBB_MapList_->GetSelectedOption());
		if (ensureMsgf(MapAssetId.IsValid(), TEXT("MapAssetId was invalid")))
		{
			SessionFindData.MapData_ = MapAssetId;
		}
		GI->FindSessions(GetOwningPlayer(), SessionFindData);
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

	SCB_SessionList_->ClearChildren();
	for (auto Result : _Results)
	{
		USessionSlotWidget* SlotWidget = CreateWidget<USessionSlotWidget>(this, SessionSlotWidgetClass_);
		SlotWidget->SetSessionData(Result);
		SCB_SessionList_->AddChild(SlotWidget);
	}
}

//맵 리스트 순회, 찾는 맵 AssetID를 가져옴
FPrimaryAssetId UJoinSessionWidget::FindMapAssetIdByMapName(const FString& _MapName)
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
#undef LOCTEXT_NAMESPACE
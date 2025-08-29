#include "SessionWidget.h"

#include "RaceGameInstance.h"
#include "SessionGameState.h"
#include "SessionPlayerController.h"
#include "Session_UserSlot.h"
#include "GameFramework/PlayerState.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableText.h"
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Engine/AssetManager.h"
#include "World/MapData.h"


void USessionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//Delegations
	VB_UserList_->ClearChildren();
	CBB_MapList_->OnSelectionChanged.AddDynamic(this, &USessionWidget::OnMapChanged);
	Btn_Ready_->OnClicked.AddDynamic(this, &USessionWidget::OnClickedReadyBtn);
	Btn_Back_->OnClicked.AddDynamic(this, &USessionWidget::OnClickedBackBtn);
	EDT_SessionName_->OnTextCommitted.AddDynamic(this, &USessionWidget::OnSessionNameChanged);
	ETB_Password_->OnTextCommitted.AddDynamic(this, &USessionWidget::OnPasswordChanged);
	CHB_IsPrivate_->OnCheckStateChanged.AddDynamic(this, &USessionWidget::OnPrivateStateChanged);
	ETB_Chat_->OnTextCommitted.AddDynamic(this, &USessionWidget::OnPlayerChatted);

	auto GS = GetWorld()->GetGameState<ASessionGameState>();
	check(GS);

	GS->OnSessionInfoUpdated_.BindUObject(this, &USessionWidget::UpdateSessionInfo);
	GS->OnPlayerListUpdated_.BindUObject(this, &USessionWidget::UpdatePlayerList);

	//Animation Count
	Count_ = FCString::Atoi(*Txt_CountDown_->GetText().ToString());
	Txt_CountDown_->SetVisibility(ESlateVisibility::Collapsed);

    
	//Map 리스트 로드
	auto& AssetManager = UAssetManager::Get();
	TArray<FPrimaryAssetId> MapAssetDatas;
	AssetManager.GetPrimaryAssetIdList("MapData", MapAssetDatas);
	AssetManager.LoadPrimaryAssets(MapAssetDatas, TArray<FName>());

	TArray<FString> TempMapNames;
	for (const auto& AssetId : MapAssetDatas)
	{
		auto MapData = Cast<UMapData>(AssetManager.GetPrimaryAssetObject(AssetId));

		//Random을 첫 요소로 고정
		if (MapData->MapDisplayName_.ToString() == "Random")
		{
			CBB_MapList_->AddOption(MapData->MapDisplayName_.ToString());
		}
		else
		{
			TempMapNames.Add(MapData->MapDisplayName_.ToString());
		}
	}
	//Random 제외한 맵 이름
	for (const FString& MapName : TempMapNames)
	{
		CBB_MapList_->AddOption(MapName);
	}
}

//카운트 다운 애니메이션 종료 후, 게임 시작
void USessionWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* _Animation)
{
	if (_Animation != Anim_CountDown_)
	{
		return;
	}
	auto PC = GetOwningPlayer<ASessionPlayerController>();
	if (nullptr == PC)
	{
		return;		
	}

	FString MapDisplayName = CBB_MapList_->GetSelectedOption();
	if (MapDisplayName.IsEmpty())
	{
		return;
	}
	auto& AssetManager = UAssetManager::Get();
	TArray<FPrimaryAssetId> MapAssetIds;
	AssetManager.GetPrimaryAssetIdList("MapData", MapAssetIds);
	
	FPrimaryAssetId SelectedMapAssetId;
	//Random인 경우
	if (MapDisplayName == "Random")
	{
		//Random 제외하고 1~Max 사이의 값
		int32 RandomIndex = FMath::RandRange(1, MapAssetIds.Num() - 1);
		auto MapData = Cast<UMapData>(AssetManager.GetPrimaryAssetObject(MapAssetIds[RandomIndex]));
		if (MapData)
		{
			SelectedMapAssetId = MapData->MapAssetId_;
		}
	}
	//일치하는 맵을 검색
	else
	{
		for (const auto& AssetId : MapAssetIds)
		{
			auto MapData = Cast<UMapData>(AssetManager.GetPrimaryAssetObject(AssetId));
			if (MapData && MapData->MapDisplayName_.ToString() == MapDisplayName)
			{
				SelectedMapAssetId = MapData->MapAssetId_;
			}
		}
	}
	PC->RequestServerTravel(SelectedMapAssetId);
}

void USessionWidget::AddChatOnLog(const FText& _Chat, const FString& _PlayerName)
{
	auto NewChat = NewObject<UTextBlock>();
	FString NewChatLog = _PlayerName + " : " + _Chat.ToString();
	NewChat->SetText(FText::FromString(NewChatLog));
	SCB_ChatLog_->AddChild(NewChat);
	SCB_ChatLog_->ScrollToEnd();
}

//모든 유저 준비되면 카운트다운
void USessionWidget::BeginCountDown()
{
	PlayAnimation(Anim_CountDown_, 0);
	Txt_CountDown_->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	auto PC = GetOwningPlayer<ASessionPlayerController>();
	if (PC && PC->HasAuthority())
	{
		PC->RequestSetSessionJoinable(false);
		EDT_SessionName_->SetIsReadOnly(true);
		CBB_MapList_->SetIsEnabled(false);
		ETB_Password_->SetIsEnabled(false);
		CHB_IsPrivate_->SetIsEnabled(false);
	}
}

//누군가 준비를 풀면 카운트다운 정지
void USessionWidget::StopCountDown()
{
	StopAnimation(Anim_CountDown_);

	Txt_CountDown_->SetText(FText::AsNumber(Count_));
	Txt_CountDown_->SetVisibility(ESlateVisibility::Collapsed);

	auto PC = GetOwningPlayer<ASessionPlayerController>();
	if (PC && PC->HasAuthority())
	{
		PC->RequestSetSessionJoinable(true);
		EDT_SessionName_->SetIsReadOnly(false);
		CBB_MapList_->SetIsEnabled(true);
		ETB_Password_->SetIsEnabled(true);
		CHB_IsPrivate_->SetIsEnabled(true);
	}
}

void USessionWidget::UpdateSessionInfo(const FSessionInfo& _SessionInfo)
{
	EDT_SessionName_->SetText(FText::FromString(_SessionInfo.SessionName_));
	ETB_Password_->SetText(FText::FromString(_SessionInfo.Password_));
	CHB_IsPrivate_->SetCheckedState(_SessionInfo.bIsPrivate_ ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	ETB_Password_->SetIsReadOnly(!_SessionInfo.bIsPrivate_);
	
	//일치하는 맵을 검색
	auto& AssetManager = UAssetManager::Get();
	TArray<FPrimaryAssetId> MapAssetIds;
	AssetManager.GetPrimaryAssetIdList(TEXT("MapData"), MapAssetIds);
	for (const auto& AssetId : MapAssetIds)
	{
		if (AssetId == _SessionInfo.SelectedMapData_)
		{
			auto MapData = Cast<UMapData>(AssetManager.GetPrimaryAssetObject(AssetId));
			CBB_MapList_->SetSelectedOption(MapData->MapDisplayName_.ToString());
			Img_Map_->SetBrushFromSoftTexture(MapData->MapThumbnail_);
		}
	}
}

void USessionWidget::UpdatePlayerList(const TArray<FSessionPlayerInfo>& _PlayerList)
{
	UpdateUIForHost();

	if (nullptr == SessionUserSlotClass_)
	{
		ensureMsgf(false, TEXT("Session User Slot Class was not set"));
		return;
	}
	
	//최대 인원 수 변경이 있었다면, UserList를 초기화
	if (VB_UserList_->GetChildrenCount() != _PlayerList.Num())
	{
		VB_UserList_->ClearChildren();

		for (const FSessionPlayerInfo& PlayerInfo : _PlayerList)
		{
			auto SessionSlot = CreateWidget<USession_UserSlot>(this, SessionUserSlotClass_);
			if (ensure(SessionSlot))
			{
				SessionSlot->SetSlot(PlayerInfo);
				VB_UserList_->AddChildToVerticalBox(SessionSlot);
			}
		}
		return;
	}

	//그렇지 않다면, 슬롯 정보 갱신
	for (uint8 i = 0; i < _PlayerList.Num(); i++)
	{
		auto UserSlot = Cast<USession_UserSlot>(VB_UserList_->GetChildAt(i));

		if (UserSlot)
		{
			UserSlot->SetSlot(_PlayerList[i]);
		}
	}
}


//Host인지 확인 후 보여질 내용 선별
void USessionWidget::UpdateUIForHost()
{
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		bool bIsHost = PC->HasAuthority();
		EDT_SessionName_->SetIsReadOnly(!bIsHost);
		ETB_Password_->SetVisibility(bIsHost ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		CHB_IsPrivate_->SetIsEnabled(bIsHost);
		CBB_MapList_->SetIsEnabled(bIsHost);
	}
}

void USessionWidget::OnMapChanged(FString _SelectedMap, ESelectInfo::Type _SelectionType)
{
	auto& AssetManager = UAssetManager::Get();
	TArray<FPrimaryAssetId> MapAssetIds;
	AssetManager.GetPrimaryAssetIdList(TEXT("MapData"), MapAssetIds);
	for (const auto& AssetId : MapAssetIds)
	{
		auto MapData = Cast<UMapData>(AssetManager.GetPrimaryAssetObject(AssetId));
		if (MapData && MapData->MapDisplayName_.ToString() == _SelectedMap)
		{
			auto GS = GetWorld()->GetGameState<ASessionGameState>();
			if (GS)
			{
				GS->SetMap(AssetId);
			}
		}
	}
}

void USessionWidget::OnClickedReadyBtn()
{
	auto PC = Cast<ASessionPlayerController>(GetOwningPlayer());

	if (PC)
	{
		PC->Server_RequestToggleReady();
	}
}

void USessionWidget::OnClickedBackBtn()
{
	auto GI = GetGameInstance<URaceGameInstance>();
	if (GI)
	{
		GI->CleanUpSession(GetOwningPlayer());
	}
}

void USessionWidget::OnSessionNameChanged(const FText& _Text, ETextCommit::Type _CommitMethod)
{
	auto PC = Cast<ASessionPlayerController>(GetOwningPlayer());

	if (PC)
	{
		PC->RequestChangeSessionName(_Text.ToString());
	}
}

void USessionWidget::OnPasswordChanged(const FText& _Text, ETextCommit::Type _CommitMethod)
{
	auto PC = Cast<ASessionPlayerController>(GetOwningPlayer());

	if (PC)
	{
		PC->RequestChangeSessionPassword(_Text.ToString());
	}
}

void USessionWidget::OnPrivateStateChanged(bool _bChecked)
{
	auto PC = Cast<ASessionPlayerController>(GetOwningPlayer());

	if (PC)
	{
		PC->RequestChangeSessionIsPrivate(_bChecked, ETB_Password_->GetText().ToString());
	}
}

void USessionWidget::OnPlayerChatted(const FText& _Text, ETextCommit::Type _CommitMethod)
{
	//Enter 입력만 사용
	if (_CommitMethod != ETextCommit::OnEnter || _Text.IsEmpty())
	{
		return;
	}

	auto PC = Cast<ASessionPlayerController>(GetOwningPlayer());
	if (PC)
	{
		PC->Server_SendChat(_Text);
	}
	ETB_Chat_->SetText(FText());
}

void USessionWidget::UpdateCountDown()
{
	int32 Count = FCString::Atoi(*Txt_CountDown_->GetText().ToString());
	Txt_CountDown_->SetText(FText::AsNumber(--Count));
}

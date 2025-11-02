#include "SessionWidget.h"

#include "RaceGameInstance.h"
#include "RaceGameResultWidget.h"
#include "SessionGameState.h"
#include "SessionPlayerController.h"
#include "Session_UserSlot.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Engine/AssetManager.h"
#include "World/MapData.h"

#define LOCTEXT_NAMESPACE "ErrorMessages"

void USessionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//Delegations
	VB_UserList_->ClearChildren();
	ETB_Laps_->OnTextChanged.AddDynamic(this, &USessionWidget::OnLapsChanged);
	ETB_Laps_->OnTextCommitted.AddDynamic(this, &USessionWidget::OnLapsCommitted);
	Btn_Ready_->OnClicked.AddDynamic(this, &USessionWidget::OnClickedReadyBtn);
	Btn_Back_->OnClicked.AddDynamic(this, &USessionWidget::OnClickedBackBtn);
	Btn_Public_->OnClicked.AddDynamic(this, &USessionWidget::OnClickedPublicBtn);
	ETB_Chat_->OnTextCommitted.AddDynamic(this, &USessionWidget::OnPlayerChatted);

	auto GS = GetWorld()->GetGameState<ASessionGameState>();
	check(GS);

	GS->OnSessionInfoUpdated_.BindUObject(this, &USessionWidget::UpdateSessionInfo);
	GS->OnPlayerListUpdated_.BindUObject(this, &USessionWidget::UpdatePlayerList);
	UpdatePlayerList(GS->GetPlayerList());

	//Animation Count
	Count_ = FCString::Atoi(*Txt_CountDown_->GetText().ToString());
	Txt_CountDown_->SetVisibility(ESlateVisibility::Collapsed);

    
	//Map 리스트 로드
	auto& AssetManager = UAssetManager::Get();
	TArray<FPrimaryAssetId> MapAssetDatas;
	AssetManager.GetPrimaryAssetIdList("MapData", MapAssetDatas);
	AssetManager.LoadPrimaryAssets(MapAssetDatas, TArray<FName>(),
		FStreamableDelegate::CreateUObject(this, &USessionWidget::OnMapsLoaded));
}

//카운트 다운 애니메이션 종료 후, 게임 시작
void USessionWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* _Animation)
{
	//애니메이션 종료가 아닌 중단의 경우
	if (_Animation != Anim_CountDown_ || bCountDownStopped_)
	{
		return;
	}
	
	auto PC = GetOwningPlayer<ASessionPlayerController>();
	if (nullptr == PC || false == PC->HasAuthority())
	{
		return;
	}

	auto GS = GetWorld()->GetGameState<ASessionGameState>();
	if (nullptr == GS)
	{
		return;
	}
	
	FSessionInfo SessionInfo = GS->GetSessionInfo();
	if (SessionInfo.Laps_ <= 0 || 10 < SessionInfo.Laps_)
	{
		PC->OpenMessageDialogue(LOCTEXT("ERROR_InvalidLaps", "Failed to start game.\n Please check the Laps value."));
		StopCountDown();
		return;
	}
	
	auto& AssetManager = UAssetManager::Get();
	TArray<FPrimaryAssetId> MapAssetIds;
	AssetManager.GetPrimaryAssetIdList("MapData", MapAssetIds);
	
	FPrimaryAssetId SelectedMapAssetId = SessionInfo.SelectedMapData_;
	auto MapData = Cast<UMapData>(AssetManager.GetPrimaryAssetObject(SelectedMapAssetId));

	if (ensureMsgf(MapData, TEXT("MapData was Invalid")))
	{
		//Random을 고른 경우, Random을 제외한 무작위 맵 선정
		if (MapData->MapDisplayName_.ToString() == "Random")
		{
			uint8 Index = FMath::RandRange(0, MapAssetIds.Num() - 2);
			
			uint8 IndexOfRandom = 0;
			for (uint8 i = 0; i < MapAssetIds.Num(); i++)
			{
				if (MapAssetIds[i] == SelectedMapAssetId)
				{
					IndexOfRandom = i;
					break;
				}
			}

			if (IndexOfRandom <= Index)
			{
				Index++;
			}

			SelectedMapAssetId = MapAssetIds[Index];
			MapData = Cast<UMapData>(AssetManager.GetPrimaryAssetObject(SelectedMapAssetId));
		}
		PC->RequestServerTravel(MapData->MapAssetId_, SessionInfo.Laps_);
	}
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
	bCountDownStopped_ = false;
	
	Txt_CountDown_->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	auto PC = GetOwningPlayer<ASessionPlayerController>();
	if (PC && PC->HasAuthority())
	{
		PC->RequestSetSessionJoinable(false);
		Btn_Public_->SetIsEnabled(false);
		ETB_Laps_->SetIsEnabled(false);
	}
}

//누군가 준비를 풀면 카운트다운 정지
void USessionWidget::StopCountDown()
{
	StopAnimation(Anim_CountDown_);
	bCountDownStopped_ = true;

	Txt_CountDown_->SetText(FText::AsNumber(Count_));
	Txt_CountDown_->SetVisibility(ESlateVisibility::Collapsed);

	auto PC = GetOwningPlayer<ASessionPlayerController>();
	if (PC && PC->HasAuthority())
	{
		PC->RequestSetSessionJoinable(true);
		Btn_Public_->SetIsEnabled(true);
		ETB_Laps_->SetIsEnabled(true);
	}
}

void USessionWidget::OnMapsLoaded()
{
	auto GS = GetWorld()->GetGameState<ASessionGameState>();
	if (nullptr == GS)
	{
		return;
	}

	FSessionInfo SessionInfo = GS->GetSessionInfo();
	FPrimaryAssetId MapId = SessionInfo.SelectedMapData_;
	
	auto& AssetManager = UAssetManager::Get();
	auto MapData = Cast<UMapData>(AssetManager.GetPrimaryAssetObject(MapId));
	if (ensureMsgf(MapData, TEXT("Map Data was nullptr")))
	{
		Img_Map_->SetBrushFromSoftTexture(MapData->MapThumbnail_);
		Txt_Map_->SetText(MapData->MapDisplayName_);
	}
}


//현재 advertisement 상태인 Session 정보를 갱신할 수 없어, 공개방 여부만을 갱신하고 있음.
//차후 인원 수 변동 / 맵 변경 등의 기능이 추가될 수 있음.
void USessionWidget::UpdateSessionInfo(const FSessionInfo& _SessionInfo)
{
	Txt_SessionName_->SetText(FText::FromString(_SessionInfo.SessionName_));
	ETB_Laps_->SetText(FText::AsNumber(_SessionInfo.Laps_));
	
	if (_SessionInfo.bIsPrivate_)
	{
		Img_Private_->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Btn_Public_->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		Img_Private_->SetVisibility(ESlateVisibility::Collapsed);
		Btn_Public_->SetVisibility(ESlateVisibility::Collapsed);
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
		Btn_Public_->SetVisibility(bIsHost ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		ETB_Laps_->SetIsEnabled(bIsHost);
	}
}

//Laps 텍스트 변경 시 검사
void USessionWidget::OnLapsChanged(const FText& _Text)
{
	APlayerController* PC = GetOwningPlayer();
	if (nullptr == PC || false == PC->HasAuthority())
	{
		return;
	}
	if (_Text.IsEmpty())
	{
		return;
	}
	
	if (false == _Text.IsNumeric())
	{
		ETB_Laps_->SetText(LastValidLaps_);
		return;
	}

	int32 Num = FCString::Atoi(*_Text.ToString());
	if (Num <= 0)
	{
		LastValidLaps_ = FText::AsNumber(1);
		ETB_Laps_->SetText(LastValidLaps_);
	}
	//최대값 10
	else if (10 < Num)
	{
		LastValidLaps_ = FText::AsNumber(10);
		ETB_Laps_->SetText(LastValidLaps_);
	}
	//이상 없음
	else
	{
		LastValidLaps_ = _Text;
	}
}

void USessionWidget::OnLapsCommitted(const FText& _Text, ETextCommit::Type _CommitType)
{
	APlayerController* PC = GetOwningPlayer();
	if (nullptr == PC || false == PC->HasAuthority())
	{
		return;
	}
	if (_Text.IsEmpty())
	{
		ETB_Laps_->SetText(LastValidLaps_);
	}
	else if (false == _Text.IsNumeric())
	{
		ETB_Laps_->SetText(LastValidLaps_);
	}
	else
	{
		int32 Num = FCString::Atoi(*_Text.ToString());
		if (Num <= 0)
		{
			LastValidLaps_ = FText::AsNumber(1);
			ETB_Laps_->SetText(LastValidLaps_);
		}
		else if (10 < Num)
		{
			LastValidLaps_ = FText::AsNumber(10);
			ETB_Laps_->SetText(LastValidLaps_);
		}
		//이상 없음
		else
		{
			LastValidLaps_ = _Text;
		}
	}

	auto GS = GetWorld()->GetGameState<ASessionGameState>();
	if (GS && ETB_Laps_->GetText().IsNumeric())
	{
		GS->SetLaps(FCString::Atoi(*ETB_Laps_->GetText().ToString()));
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

void USessionWidget::OnClickedPublicBtn()
{
	auto PC = Cast<ASessionPlayerController>(GetOwningPlayer());

	if (PC && PC->HasAuthority())
	{
		PC->Server_RequestMakeSessionPublic();
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

#undef LOCTEXT_NAMESPACE

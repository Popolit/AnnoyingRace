#include "SessionPlayerController.h"

#include "EnhancedInputComponent.h"
#include "RaceGameInstance.h"
#include "SessionGameMode.h"
#include "SessionGameState.h"
#include "Engine/AssetManager.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "UI/MessageDialogueWidget.h"
#include "UI/SessionWidget.h"
#include "World/RaceWorldSettings.h"

#define LOCTEXT_NAMESPACE "ErrorMessages"

class URaceGameInstance;

ASessionPlayerController::ASessionPlayerController()
{
	bReplicates = true;
}

void ASessionPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(SessionWidgetClass_);

	if (IsLocalPlayerController())
	{
		SessionWidget_ = CreateWidget<USessionWidget>(this, SessionWidgetClass_);
		check(SessionWidget_);

		//BGM재생
		if (ensureMsgf(SoundMix_, TEXT("Session Controller's SoundMix was not set")))
		{
			UGameplayStatics::PushSoundMixModifier(this, SoundMix_);
			auto WorldSettings = Cast<ARaceWorldSettings>(GetWorldSettings());
			if (ensureMsgf(WorldSettings && WorldSettings->WorldBGM_, TEXT("World Settings' BGM was not set")))
			{
				UGameplayStatics::PlaySound2D(this, WorldSettings->WorldBGM_);
			}
		}
		SessionWidget_->AddToViewport(0);

		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(SessionWidget_->TakeWidget());
		SetInputMode(InputMode);
		bShowMouseCursor = true;
	}
}

void ASessionPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	auto IC = Cast<UEnhancedInputComponent>(InputComponent);
	if (ensureMsgf(IC, TEXT("Lobby's InputComponent was nullptr")))
	{
		if (ensureMsgf(IA_Cancel_, TEXT("%s's IA_Cancel was nullptr"), *GetName()))
		{
			//IC->BindAction(IA_Cancel_, ETriggerEvent::Triggered, this, &ASessionPlayerController::OnCancelButtonPressed);
		}
	}
}

void ASessionPlayerController::Server_RequestToggleReady_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		auto GS = World->GetGameStateChecked<ASessionGameState>();

		GS->TogglePlayerIsReady(PlayerState);
	}
}


void ASessionPlayerController::Server_SendChat_Implementation(const FText& _Chat)
{
	UE_LOG(LogTemp, Warning, TEXT("Server_SendChat called: %s"), *_Chat.ToString());
	auto GM = GetWorld()->GetAuthGameMode<ASessionGameMode>();

	if (ensureMsgf(GM, TEXT("Session Game Mode was nullptr")))
	{
		GM->RequestChat(_Chat, PlayerState->GetPlayerName());
	}
}

void ASessionPlayerController::Client_AddChatOnLog_Implementation(const FText& _Chat, const FString& _ChatterName)
{
	if (SessionWidget_)
	{
		SessionWidget_->AddChatOnLog(_Chat, _ChatterName);
	}
}

void ASessionPlayerController::Client_StartCountDownForPlay_Implementation()
{
	if (SessionWidget_)
	{
		SessionWidget_->BeginCountDown();
	}
}

void ASessionPlayerController::Client_StopCountDownForPlay_Implementation()
{
	if (SessionWidget_)
	{
		SessionWidget_->StopCountDown();
	}
}

void ASessionPlayerController::RequestSetSessionJoinable(bool _bCanJoin)
{
	//이 함수는 Host가 호출하기 때문에 ServerRPC 대신 HasAuthority 체크만 함
	if (HasAuthority())
	{
		auto GM = GetWorld()->GetAuthGameMode<ASessionGameMode>();

		if (ensureMsgf(GM, TEXT("Session Game Mode was nullptr")))
		{
			GM->SetSessionJoinable(_bCanJoin);
		}
	}
}

void ASessionPlayerController::RequestChangeSessionName(const FString& _SessionName)
{
	//이 함수는 Host가 호출하기 때문에 ServerRPC 대신 HasAuthority 체크만 함
	if (HasAuthority())
	{
		auto GM = GetWorld()->GetAuthGameMode<ASessionGameMode>();

		if (ensureMsgf(GM, TEXT("Session Game Mode was nullptr")))
		{
			GM->ChangeSessionName(_SessionName);
		}
	}
}

void ASessionPlayerController::RequestChangeSessionPassword(const FString& _SessionPassword)
{
	//이 함수는 Host가 호출하기 때문에 ServerRPC 대신 HasAuthority 체크만 함
	if (HasAuthority())
	{
		auto GM = GetWorld()->GetAuthGameMode<ASessionGameMode>();

		if (ensureMsgf(GM, TEXT("Session Game Mode was nullptr")))
		{
			GM->ChangeSessionPassword(_SessionPassword);
		}
	}
}

void ASessionPlayerController::RequestChangeSessionIsPrivate(bool _bIsPrivate, const FString& _SessionPassword)
{
	//이 함수는 Host가 호출하기 때문에 ServerRPC 대신 HasAuthority 체크만 함
	if (HasAuthority())
	{
		auto GM = GetWorld()->GetAuthGameMode<ASessionGameMode>();

		if (ensureMsgf(GM, TEXT("Session Game Mode was nullptr")))
		{
			GM->ChangeSessionIsPrivate(_bIsPrivate, _SessionPassword);
		}
	}
}

//해당하는 맵으로 게임 시작
void ASessionPlayerController::RequestServerTravel(const FPrimaryAssetId& _MapId)
{
	if (HasAuthority() && ensureMsgf(_MapId.IsValid(), TEXT("Map was nullptr")))
	{
		URaceGameInstance* GI = GetGameInstance<URaceGameInstance>();
		ASessionGameState* GS = GetWorld()->GetGameState<ASessionGameState>();
		auto GM = GetWorld()->GetAuthGameMode<ASessionGameMode>();
		if (nullptr == GM || nullptr == GI || nullptr == GS)
		{
			return;
		}
		
		UAssetManager& AssetManager = UAssetManager::Get();

		FAssetData AssetData;
		if (AssetManager.GetPrimaryAssetData(_MapId, AssetData))
		{
			FString MapPath = AssetData.PackageName.ToString();
			FString URL = MapPath + "?listen";

			if (false == GM->CanServerTravel(URL, true))
			{
				FText ErrorMessage = LOCTEXT("Error_FailedServerTravel", "Failed to start game.");
				OpenMessageDialogue(ErrorMessage);
				return;
			}
			GI->SetRacePlayerCount(GS->PlayerArray.Num());
			GM->ProcessServerTravel(URL, true);
		}
	}
}

void ASessionPlayerController::OpenMessageDialogue(const FText& _Message)
{
	if (nullptr == MessageDialogueWidget_)
	{
		if (ensureMsgf(MessageDialogueWidgetClass_, TEXT("Session's Dialogue Widget was not set")))
		{
			MessageDialogueWidget_ = CreateWidget<UMessageDialogueWidget>(this, MessageDialogueWidgetClass_);
			MessageDialogueWidget_->SetMessage(_Message);
			MessageDialogueWidget_->OnClickedClose_.BindUObject(this, &ASessionPlayerController::CloseMessageDialogue);
		}
	}
	
	if (ensureMsgf(MessageDialogueWidget_, TEXT("Dialogue Widget was nullptr")))
	{
		if (false == MessageDialogueWidget_->IsInViewport())
		{
			MessageDialogueWidget_->AddToViewport(2);
			MessageDialogueWidget_->SetMessage(_Message);
		}
		else
		{
			MessageDialogueWidget_->SetVisibility(ESlateVisibility::Visible);
		}
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(MessageDialogueWidget_->TakeWidget());
		SetInputMode(InputMode);
	}
}

void ASessionPlayerController::CloseMessageDialogue()
{
	if (MessageDialogueWidget_)
	{
		MessageDialogueWidget_->SetVisibility(ESlateVisibility::Collapsed);
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(SessionWidget_->TakeWidget());
		SetInputMode(InputMode);
	}
}

#undef LOCTEXT_NAMESPACE

#include "RacePlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "RaceGameMode.h"
#include "RaceGameState.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "RacePlayerState.h"
#include "Components/AudioComponent.h"
#include "GameFramework/GameUserSettings.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "UI/CharacterDiedWidget.h"
#include "UI/DrawCharacterWidget.h"
#include "UI/CountDownWidget.h"
#include "UI/ExitDialogueWidget.h"
#include "UI/OptionWidget.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "World/RaceWorldSettings.h"
#include "World/TrackSplineActor.h"


ARacePlayerController::ARacePlayerController()
{
	AudioComponent_ = CreateDefaultSubobject<UAudioComponent>("Audio");
	AudioComponent_->SetupAttachment(GetRootComponent());
}

void ARacePlayerController::BeginPlay()
{
	Super::BeginPlay();

	//BGM재생
	if (ensureMsgf(SoundMix_, TEXT("Race Controller's SoundMix was not set")))
	{
		UGameplayStatics::PushSoundMixModifier(this, SoundMix_);
		auto WorldSettings = Cast<ARaceWorldSettings>(GetWorldSettings());
		if (ensureMsgf(WorldSettings && WorldSettings->WorldBGM_, TEXT("World Settings' BGM was not set")))
		{
			check(AudioComponent_);
			AudioComponent_->SetSound(WorldSettings->WorldBGM_);
			AudioComponent_->Play();
		}
	}
}

void ARacePlayerController::OnPossess(APawn* _Pawn)
{
	Super::OnPossess(_Pawn);

	if (HasAuthority())
	{
		//관전 중(사망)의 경우 제외
		auto CastedPawn = Cast<ASpectatorPawn>(_Pawn);
		if(nullptr == _Pawn || CastedPawn)
		{
			return;
		}

		//0.2초에 한번 거리를 갱신
		GetWorldTimerManager().SetTimer(
			DistanceUpdateTimerHandle_,
			this,
			&ARacePlayerController::UpdateDistanceAlongSpline,
			0.2f,
			true 
		);
	}
}

void ARacePlayerController::OnUnPossess()
{
	GetWorldTimerManager().ClearTimer(DistanceUpdateTimerHandle_);

	Super::OnUnPossess();
}

void ARacePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	auto IC = Cast<UEnhancedInputComponent>(InputComponent);
	if (ensureMsgf(IC, TEXT("RacePC's InputComponent was nullptr")))
	{
		auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(IMC_Default_, 0);

			if (auto UserSettings = Subsystem->GetUserSettings())
			{
				UserSettings->RegisterInputMappingContext(IMC_Character_);
			}
		}
		if (ensureMsgf(IA_ToggleMenu_, TEXT("%s's IA_ToggleMenu_ was nullptr"), *GetName()))
		{
			IC->BindAction(IA_ToggleMenu_, ETriggerEvent::Triggered, this, &ARacePlayerController::OpenMainMenu);
		}
	}
}

//클라이언트 준비됨
void ARacePlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	Server_NotifyPlayerIsReady();
}

void ARacePlayerController::Client_PlaySound2D_Implementation(USoundCue* _Sound)
{
	UGameplayStatics::PlaySound2D(this, _Sound);
}

void ARacePlayerController::SetSpectatorMode(const FTransform& _TransformToSpectate)
{
	if (HasAuthority())
	{
		ARaceGameMode* GM = Cast<ARaceGameMode>(UGameplayStatics::GetGameMode(this));
		check(GM);

		if(nullptr == GM->SpectatorClass)
		{
			ensure(false);
			return;
		}
		auto SpawnedPawn = GetWorld()->SpawnActor<ASpectatorPawn>(GM->SpectatorClass, _TransformToSpectate);
		Possess(SpawnedPawn);
	}
}

void ARacePlayerController::Client_EnableCharacterInput_Implementation()
{
	if (false == bEnableCharacterInput_)
	{
		auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(IMC_Character_, 1);
		}
		bEnableCharacterInput_ = true;
	}
}

void ARacePlayerController::Client_DisableCharacterInput_Implementation()
{
	if (bEnableCharacterInput_)
	{
		auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->RemoveMappingContext(IMC_Character_);
		}
		bEnableCharacterInput_ = false;
	}
}


void ARacePlayerController::Client_ShowCharacterDiedWidget_Implementation()
{
	if (CharacterDiedWidget_)
	{
		CharacterDiedWidget_->SetWidget();
		CharacterDiedWidget_->ShowWidget();
	}
	else if (ensureMsgf(CharacterDiedWidgetClass_, TEXT("WidgetClass was null")))
	{
		CharacterDiedWidget_ = CreateWidget<UCharacterDiedWidget>(this, CharacterDiedWidgetClass_);
		if (CharacterDiedWidget_)
		{
			CharacterDiedWidget_->SetWidget();
			CharacterDiedWidget_->OnCharacterDIedAnimationFinished_.BindUObject(this, &ARacePlayerController::OnCharacterDiedAnimationFinished);
			CharacterDiedWidget_->AddToViewport();
			CharacterDiedWidget_->ShowWidget();
		}
	}
}

void ARacePlayerController::Client_StartRaceCountdown_Implementation()
{
	if (ensureMsgf(CountDownWidgetClass_, TEXT("WidgetClass was null")))
	{
		CountdownWidget_ = CreateWidget<UCountDownWidget>(this, CountDownWidgetClass_);
		if (CountdownWidget_)
		{
			CountdownWidget_->OnCountdownAnimationFinished_.BindUObject(this, &ARacePlayerController::OnCountDownAnimationFinished);
			CountdownWidget_->AddToViewport();
		}
	}
}

void ARacePlayerController::Client_ShowCharacterDrawResult_Implementation(const UCharacterData* _DrawnCharacterData)
{
	if(DrawCharacterWidget_)
	{
		DrawCharacterWidget_->SetWidget(_DrawnCharacterData);
		DrawCharacterWidget_->ShowWidget();
	}
	else if (ensureMsgf(DrawCharacterWidgetClass_, TEXT("WidgetClass was null")))
	{
		DrawCharacterWidget_ = CreateWidget<UDrawCharacterWidget>(this, DrawCharacterWidgetClass_);
		if (DrawCharacterWidget_)
		{
			DrawCharacterWidget_->SetWidget(_DrawnCharacterData);
			DrawCharacterWidget_->OnDrawAnimationFinished_.BindUObject(this, &ARacePlayerController::OnDrawAnimationFinished);
			DrawCharacterWidget_->AddToViewport();
			DrawCharacterWidget_->ShowWidget();
		}
	}
}

void ARacePlayerController::OpenMainMenu()
{
	if (nullptr == RaceMenuWidget_ && ensureMsgf(RaceMenuWidgetClass_, TEXT("WidgetClass was null")))
	{
		RaceMenuWidget_ = CreateWidget(this, RaceMenuWidgetClass_);
		RaceMenuWidget_->AddToViewport(30);
	}
	OpenInteractableWidget(RaceMenuWidget_);
}

void ARacePlayerController::ExitGame()
{
	//TODO : Lobby로 돌아가기
	ConsoleCommand("quit");
}

void ARacePlayerController::CloseMainMenu()
{
	CloseInteractableWidget(RaceMenuWidget_);
}

void ARacePlayerController::OpenOptionMenu()
{
	if(nullptr == OptionWidget_ && ensureMsgf(OptionWidgetClass_, TEXT("WidgetClass was null")))
	{
		OptionWidget_ = CreateWidget<UOptionWidget>(this, OptionWidgetClass_);
	}

	if (ensureMsgf(OptionWidget_, TEXT("Option Widget was nullptr")))
	{
		OptionWidget_->OnBackBtnClicked_.BindUObject(this, &ARacePlayerController::CloseOptionMenu);
		OptionWidget_->AddToViewport(30);
		CloseMainMenu();
		OpenInteractableWidget(OptionWidget_);
	}
}

void ARacePlayerController::OpenExitDialogue()
{
	if(nullptr == ExitDialogueWidget_ && ensureMsgf(ExitDialogueWidgetClass_, TEXT("WidgetClass was null")))
	{
		ExitDialogueWidget_ = CreateWidget<UExitDialogueWidget>(this, ExitDialogueWidgetClass_);
	}
	if (ensureMsgf(ExitDialogueWidget_, TEXT("Exit Widget was nullptr")))
	{
		ExitDialogueWidget_->OnYesBtnClicked_.BindUObject(this, &ARacePlayerController::ExitGame);
		ExitDialogueWidget_->OnNoBtnClicked_.BindUObject(this, &ARacePlayerController::CloseExitDialogue);
		ExitDialogueWidget_->AddToViewport(30);
		CloseMainMenu();
		OpenInteractableWidget(ExitDialogueWidget_);
	}
}

void ARacePlayerController::CloseOptionMenu()
{
	CloseInteractableWidget(OptionWidget_);

	auto UserSettings = GEngine->GetGameUserSettings();

	if (ensure(UserSettings))
	{
		UserSettings->SaveSettings();
	}
}

void ARacePlayerController::CloseExitDialogue()
{
	CloseInteractableWidget(ExitDialogueWidget_);
}

void ARacePlayerController::Client_PlaySequence_Implementation(const FName& _SequenceName)
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, ALevelSequenceActor::StaticClass(), _SequenceName, OutActors);

	if (ensureMsgf(OutActors.IsValidIndex(0), TEXT("Level SequenceTag was Invalid")))
	{
		ALevelSequenceActor* SequenceActor = Cast<ALevelSequenceActor>(OutActors[0]);
		if (ensure(SequenceActor && SequenceActor->GetSequencePlayer()))
		{
			SequenceActor->GetSequencePlayer()->Play();
		}
	}
}

void ARacePlayerController::Client_OpenWaitingPlayersUI_Implementation()
{
	if ( nullptr == WaitingPlayersWidget_ && ensureMsgf( WaitingPlayersWidgetClass_ , TEXT( "WidgetClass was null" ) ) )
	{
		WaitingPlayersWidget_ = CreateWidget( this , WaitingPlayersWidgetClass_ );
		WaitingPlayersWidget_->AddToViewport();
	}
	OpenInteractableWidget( WaitingPlayersWidget_ );
}

void ARacePlayerController::Client_CloseWaitingPlayersUI_Implementation()
{
	if(WaitingPlayersWidget_ )
	{
		WaitingPlayersWidget_->RemoveFromParent();
		FInputModeGameOnly InputMode;
		SetInputMode( InputMode );
		bShowMouseCursor = false;
	}
}


void ARacePlayerController::Server_RequestDrawCharacter_Implementation()
{
	ARaceGameMode* GM = Cast<ARaceGameMode>(UGameplayStatics::GetGameMode(this));
	GM->DrawNewCharacter(this);
}


void ARacePlayerController::Server_RequestSpawnCharacter_Implementation()
{
	ARaceGameMode* GM = Cast<ARaceGameMode>(UGameplayStatics::GetGameMode(this));
	GM->SpawnNewCharacter(this);
}


void ARacePlayerController::Server_NotifyPlayerIsReady_Implementation()
{
	ARaceGameMode* GM = Cast<ARaceGameMode>(UGameplayStatics::GetGameMode(this));
	GM->AddReadiedPlayerCount();
}

void ARacePlayerController::OnDrawAnimationFinished()
{
	Server_RequestSpawnCharacter();
}


void ARacePlayerController::OnCountDownAnimationFinished()
{
	ARaceGameState* GS = GetWorld()->GetGameState<ARaceGameState>();
	check(GS);

	GS->HandleStartRace();
	Client_EnableCharacterInput();

	CountdownWidget_->RemoveFromParent();

	if(ensureMsgf(PlayerHUDWidgetClass_, TEXT("WidgetClass was null")))
	{
		PlayerHUDWidget_ = CreateWidget(this, PlayerHUDWidgetClass_);
		PlayerHUDWidget_->AddToViewport();
	}
}

void ARacePlayerController::OnCharacterDiedAnimationFinished()
{
	Server_RequestDrawCharacter();
}

void ARacePlayerController::UpdateDistanceAlongSpline()
{
	ARacePlayerState* PS = GetPlayerState<ARacePlayerState>();
	APawn* MyPawn = GetPawn();

	if (ensure(PS && MyPawn))
	{
		if (nullptr == TrackSplineActor_)
		{
			TrackSplineActor_ = Cast<ATrackSplineActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ATrackSplineActor::StaticClass()));
		}

		if (TrackSplineActor_)
		{
			const float CurrentDistance = TrackSplineActor_->GetTotalDistance(MyPawn->GetActorLocation());

			PS->SetTotalDistance(CurrentDistance);
		}
	}
}

void ARacePlayerController::OpenInteractableWidget(UUserWidget* _Widget)
{
	if (_Widget)
	{
		_Widget->SetVisibility(ESlateVisibility::Visible);
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(_Widget->TakeWidget());
		SetInputMode(InputMode);
		bShowMouseCursor = true;
	}
}

void ARacePlayerController::CloseInteractableWidget(UUserWidget* _Widget)
{
	if (_Widget)
	{
		_Widget->SetVisibility(ESlateVisibility::Collapsed);
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		bShowMouseCursor = false;
	}
}


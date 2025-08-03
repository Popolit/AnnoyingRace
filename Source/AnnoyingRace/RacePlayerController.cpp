#include "RacePlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "RaceGameMode.h"
#include "RaceGameState.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "RacePlayerState.h"
#include "GameFramework/GameUserSettings.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "UI/CharacterDiedWidget.h"
#include "UI/DrawCharacterWidget.h"
#include "UI/CountDownWidget.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "World/TrackSplineActor.h"

ARacePlayerController::ARacePlayerController()
{
	InputComponent = CreateDefaultSubobject<UEnhancedInputComponent>("InputComponent");
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
	auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(IMC_Default_, 0);

		if (auto UserSettings = Subsystem->GetUserSettings())
		{
			UserSettings->RegisterInputMappingContext(IMC_Character_);
		}
	}
	auto IC = Cast<UEnhancedInputComponent>(InputComponent);
	if (ensureMsgf(IA_ToggleMenu_, TEXT("%s's IA_ToggleMenu_ was nullptr"), *GetName()))
	{
		IC->BindAction(IA_ToggleMenu_, ETriggerEvent::Triggered, this, &ARacePlayerController::OpenMainMenu);
	}
}

void ARacePlayerController::PlaySequence(const FName& _SequenceName)
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
	//TODO : 차후 로비가 구현된 후 로비로 돌아가도록 수정
	//TODO : 클라이언트, 호스트가 게임을 종료할 때 적절한 핸들
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
		OptionWidget_ = CreateWidget(this, OptionWidgetClass_);
		OptionWidget_->AddToViewport(30);
	}

	//Load UserSettings
	auto UserSettings = GEngine->GetGameUserSettings();
	{
		UserSettings->LoadSettings();
	}
	CloseMainMenu();
	OpenInteractableWidget(OptionWidget_);
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

void ARacePlayerController::OpenConfirmExitGameDialog()
{
	if(nullptr == ConfirmExitGameWidget_ && ensureMsgf(ConfirmExitGameWidgetClass_, TEXT("WidgetClass was null")))
	{
		ConfirmExitGameWidget_ = CreateWidget(this, ConfirmExitGameWidgetClass_);
		ConfirmExitGameWidget_->AddToViewport(30);
	}
	CloseMainMenu();
	OpenInteractableWidget(ConfirmExitGameWidget_);
}

void ARacePlayerController::CloseConfirmExitGameDialog()
{
	CloseInteractableWidget(ConfirmExitGameWidget_);
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


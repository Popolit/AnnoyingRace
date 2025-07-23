#include "RacePlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "RaceGameMode.h"
#include "RaceGameState.h"
#include "LevelSequenceActor.h"
#include "RacePlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CharacterDiedWidget.h"
#include "UI/DrawCharacterWidget.h"
#include "UI/CountDownWidget.h"
#include "World/TrackSplineActor.h"


void ARacePlayerController::PlaySequence(const FName& _SequenceName)
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, ALevelSequenceActor::StaticClass(), _SequenceName, OutActors);

	if (ensureMsgf(OutActors.IsValidIndex(0), TEXT("Level SequenceTag was Invalid")))
	{
		ALevelSequenceActor* SequenceActor = Cast<ALevelSequenceActor>(OutActors[0]);
		if (ensure(SequenceActor && SequenceActor->SequencePlayer))
		{
			SequenceActor->SequencePlayer->Play();
		}
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

	CountdownWidget_->RemoveFromViewport();
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

	if (PS && MyPawn)
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


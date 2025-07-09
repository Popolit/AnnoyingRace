#include "RacePlayerController.h"

#include "RaceGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequenceActor.h"
#include "UI/DrawCharacterWidget.h"
#include "UI/CountDownWidget.h"


void ARacePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ALevelSequenceActor::StaticClass(), FName("IntroSequence"), FoundActors);

		if (ensureMsgf(FoundActors.Num() > 0, TEXT("Cannot find IntroSequence Actor")))
		{
			ALevelSequenceActor* IntroSequenceActor = Cast<ALevelSequenceActor>(FoundActors[0]);
			IntroSequenceActor->SetReplicates(true);
			ULevelSequencePlayer* SequencePlayer = IntroSequenceActor->GetSequencePlayer();
			if (SequencePlayer)
			{
				SequencePlayer->OnFinished.AddDynamic(this, &ARacePlayerController::OnIntroSequenceFinished);
				SequencePlayer->Play();
			}
		}
	}
}


void ARacePlayerController::Multicast_OnIntroSequenceFinished_Implementation()
{
	Server_RequestDrawCharacter();
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


void ARacePlayerController::OnIntroSequenceFinished()
{
	Multicast_OnIntroSequenceFinished();
}

void ARacePlayerController::Client_StartRaceCountdown_Implementation()
{
	if (ensureMsgf(CountDownWidgetClass_, TEXT("WidgetClass was null")))
	{
		CountdownWidget_ = CreateWidget<UCountDownWidget>(this, CountDownWidgetClass_);
		if (CountdownWidget_)
		{
			CountdownWidget_->AddToViewport();
			CountdownWidget_->OnCountdownAnimationFinished_.BindUObject(this, &ARacePlayerController::OnCountDownAnimationFinished);
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

void ARacePlayerController::OnDrawAnimationFinished()
{
	Server_RequestSpawnCharacter();
}


void ARacePlayerController::OnCountDownAnimationFinished()
{
	CountdownWidget_->RemoveFromViewport();
	CountdownWidget_->RemoveFromParent();
}

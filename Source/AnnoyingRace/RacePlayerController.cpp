#include "RacePlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequenceActor.h"
#include "RaceGameMode.h"

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
			}
		}
	}
}


void ARacePlayerController::Multicast_OnIntroSequenceFinished_Implementation()
{
	if(IsLocalController())
	{
		if (ensureMsgf(DrawCharacterWidgetClass_, TEXT("WidgetClass was null")))
		{
			DrawCharacterWidget_ = CreateWidget(this, DrawCharacterWidgetClass_);
			if (DrawCharacterWidget_)
			{
				DrawCharacterWidget_->AddToViewport();
			}
		}
		Server_RequestDrawCharacter();
	}
}

void ARacePlayerController::Server_RequestDrawCharacter_Implementation()
{
	ARaceGameMode* GM = Cast<ARaceGameMode>(UGameplayStatics::GetGameMode(this));
	GM->DrawNewCharacter(0, this);
}

void ARacePlayerController::OnIntroSequenceFinished()
{
	Multicast_OnIntroSequenceFinished();
}

void ARacePlayerController::StartRaceCountdown_Implementation(float _CountDownStartTime)
{
	if (ensureMsgf(CountdownWidgetClass_, TEXT("WidgetClass was null")))
	{
		CountdownWidget_ = CreateWidget(this, CountdownWidgetClass_);
		if (CountdownWidget_)
		{
			//CountdownWidget_->AddToViewport();
		}
	}
}

void ARacePlayerController::DrawCharacter_Implementation()
{
	
}

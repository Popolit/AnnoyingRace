#include "RaceGameMode.h"

#include "RaceGameState.h"
#include "RacePlayerState.h"
#include "Characters/RaceSpectatorPawn.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "World/TrackSplineActor.h"

ARaceGameMode::ARaceGameMode()
{
	GameStateClass = ARaceGameState::StaticClass();
	PlayerStateClass = ARacePlayerState::StaticClass();
	DefaultPawnClass = ARaceSpectatorPawn::StaticClass();
}


void ARaceGameMode::StartPlay()
{
	Super::StartPlay();

	ShuffleCharacterQueue();

	TrackSpline_ = Cast<ATrackSplineActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ATrackSplineActor::StaticClass()));

	GetGameState<ARaceGameState>()->SetMaxCheckPointCount(TrackSpline_->GetNumberOfCheckPoints());
}

void ARaceGameMode::PostLogin(APlayerController* _NewPlayer)
{
	Super::PostLogin(_NewPlayer);

	ARacePlayerState* RacePlayerState = _NewPlayer->GetPlayerState<ARacePlayerState>();
	check(RacePlayerState);
	RacePlayerState->SetTargetCheckPointIndex(1);
}

void ARaceGameMode::DrawNewCharacter(uint8 _CheckPointIndex, APlayerController* _PC)
{
	check(_PC);

	if (CharacterQueue_.IsEmpty())
	{
		ShuffleCharacterQueue();
	}
	const TSubclassOf<ACharacter> NextCharacterClass = *CharacterQueue_.Peek();
	CharacterQueue_.Pop();

	check(NextCharacterClass);

	const FTransform SpawnTransform = TrackSpline_->GetSplinePointTransform(_CheckPointIndex);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = _PC;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ACharacter* NextCharacter = Cast<ACharacter>(GetWorld()->SpawnActor(*NextCharacterClass, &SpawnTransform, SpawnParams));
	if (ensureMsgf(NextCharacter, TEXT("Character Spawn Failed")))
	{
		APawn* PassedPawn = _PC->GetPawn();
		if (_PC->GetPawn())
		{
			PassedPawn->Destroy();
		}
		//_PC->SetPawn(NextCharacter);
		_PC->Possess(NextCharacter);

		// 디버깅 로그
		if (NextCharacter->GetOwner() == _PC)
		{
			UE_LOG(LogTemp, Warning, TEXT("Character %s is now correctly owned by %s"), *NextCharacter->GetName(), *_PC->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Character %s ownership FAILED! Owner is %s"), *NextCharacter->GetName(), *_PC->GetName());
		}
	}
}

void ARaceGameMode::HandleCheckPointPassed(uint8 _CheckPointIndex, APlayerController* _PC)
{
	ARacePlayerState* PS = _PC->GetPlayerState<ARacePlayerState>();
	check(PS);

	uint8 TargetCheckPointIndex = PS->GetTargetCheckPointIndex();
	if(_CheckPointIndex != TargetCheckPointIndex)
	{
		return;
	}

	TargetCheckPointIndex++;
	TargetCheckPointIndex %= GetGameState<ARaceGameState>()->GetMaxCheckPointCount();
	PS->SetTargetCheckPointIndex(TargetCheckPointIndex);
	if(_CheckPointIndex == 0)
	{
		PS->IncreaseLap();
		if (GetGameState<ARaceGameState>()->GetMaxLap() <= PS->GetLaps())
		{
			//완주 처리 코드
			return;
		}
	}

	DrawNewCharacter(_CheckPointIndex, _PC);
}

TSubclassOf<ACharacter> ARaceGameMode::PopNextCharacter()
{
	//다음 캐릭터 클래스 선택
	const TSubclassOf<ACharacter> CharacterClass = *CharacterQueue_.Peek();
	CharacterQueue_.Pop();

	return CharacterClass;
}

void ARaceGameMode::StartRaceCountDown()
{
}

void ARaceGameMode::RemoveBlockVolumes()
{
}

void ARaceGameMode::ShuffleCharacterQueue()
{
	UINT LeftCount = CharacterPool_.Num();
	TArray<bool> IsSelected;
	IsSelected.SetNumZeroed(LeftCount);
	while(LeftCount--)
	{
		UINT Index = FMath::RandRange(0, LeftCount);
		while(IsSelected[Index])
		{
			Index = (Index + 1) % CharacterPool_.Num();
		}
		IsSelected[Index] = true;
		CharacterQueue_.Enqueue(CharacterPool_[Index]);
	}
}

void ARaceGameMode::ChangeCharacter()
{
	
}

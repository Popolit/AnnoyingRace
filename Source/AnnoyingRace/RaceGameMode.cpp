#include "RaceGameMode.h"

#include "RaceGameState.h"
#include "RacePlayerController.h"
#include "RacePlayerState.h"
#include "Characters/RaceSpectatorPawn.h"
#include "Characters/PlayableCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "World/TrackSplineActor.h"
#include "Characters/CharacterData.h"

ARaceGameMode::ARaceGameMode()
{
	GameStateClass = ARaceGameState::StaticClass();
	PlayerStateClass = ARacePlayerState::StaticClass();
	DefaultPawnClass = nullptr;

	bRaceStarted = false;
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

void ARaceGameMode::DrawNewCharacter(APlayerController* _PC)
{
	ARacePlayerController* PC = Cast<ARacePlayerController>(_PC);
	check(PC);

	if (CharacterQueue_.IsEmpty())
	{
		ShuffleCharacterQueue();
	}

	UCharacterData* NextCharacterData = *CharacterQueue_.Peek();
	CharacterQueue_.Pop();

	check(NextCharacterData);

	PlayersCharacterInfo_.FindOrAdd(_PC) = NextCharacterData;
	PC->Client_ShowCharacterDrawResult(NextCharacterData);

}

void ARaceGameMode::SpawnNewCharacter(APlayerController* _PC)
{
	UCharacterData* CharacterData = *PlayersCharacterInfo_.Find(_PC);

	if (nullptr == CharacterData)
	{
		ensure(true);
		return;
	}
	TSubclassOf<ACharacter> NextCharacterClass = CharacterData->GetCharacterClass();

	FTransform SpawnTransform;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = _PC;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	//Race가 진행되고 있을 때의 Spawn 위치
	if (bRaceStarted)
	{
		ARacePlayerState* PS = _PC->GetPlayerState<ARacePlayerState>();
		check(PS);

		uint8 CheckPointIndex = PS->GetTargetCheckPointIndex();
		CheckPointIndex++;
		CheckPointIndex %= GetGameState<ARaceGameState>()->GetMaxCheckPointCount();
		PS->SetTargetCheckPointIndex(CheckPointIndex);
		SpawnTransform = TrackSpline_->GetSplinePointTransform(CheckPointIndex);
	}
	//레이스 시작 후 첫 출발시 Spawn 위치
	else
	{
		const AActor* PlayerStart = FindPlayerStart(_PC);
		if (PlayerStart)
		{
			SpawnTransform = PlayerStart->GetTransform();
		}
	}

	ACharacter* NextCharacter = GetWorld()->SpawnActor<ACharacter>(NextCharacterClass, SpawnTransform, SpawnParams);
	check(NextCharacter);
	_PC->Possess(NextCharacter);

	if (false == bRaceStarted)
	{
		ARacePlayerController* RacePlayerController = Cast<ARacePlayerController>(_PC);
		if(RacePlayerController)
		{
			RacePlayerController->Client_StartRaceCountdown();
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

	
	if(_CheckPointIndex == 0)
	{
		PS->IncreaseLap();
		if (GetGameState<ARaceGameState>()->GetMaxLap() <= PS->GetLaps())
		{
			//완주 처리 코드
			return;
		}
	}

	APawn* PassedPawn = _PC->GetPawn();
	if (PassedPawn)
	{
		PassedPawn->Destroy();
	}

	DrawNewCharacter(_PC);
}

TObjectPtr<UCharacterData> ARaceGameMode::PopNextCharacter()
{
	//다음 캐릭터 클래스 선택
	const TObjectPtr<UCharacterData> CharacterData = *CharacterQueue_.Peek();
	CharacterQueue_.Pop();

	return CharacterData;
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

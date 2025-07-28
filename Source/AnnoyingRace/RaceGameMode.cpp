#include "RaceGameMode.h"

#include "LevelSequenceActor.h"
#include "RaceGameState.h"
#include "RacePlayerState.h"
#include "RacePlayerController.h"
#include "Characters/PlayableCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "World/TrackSplineActor.h"
#include "Characters/CharacterData.h"

ARaceGameMode::ARaceGameMode()
{
	GameStateClass = ARaceGameState::StaticClass();
	PlayerStateClass = ARacePlayerState::StaticClass();
	DefaultPawnClass = nullptr;
	UnReadiedPlayerCount_ = 0;
}

void ARaceGameMode::StartPlay()
{
	Super::StartPlay();

	ShuffleCharacterQueue();

	TrackSpline_ = Cast<ATrackSplineActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ATrackSplineActor::StaticClass()));
	check(TrackSpline_);

	ARaceGameState* GS = GetGameState<ARaceGameState>();
	check(GS);

	GS->SetMaxCheckPointCount(TrackSpline_->GetNumberOfCheckPoints());
}

void ARaceGameMode::StartMatch()
{
	Super::StartMatch();

	ARaceGameState* GS = GetGameState<ARaceGameState>();
	check(GS);

	//인트로 연출 재생
	for(auto Player : GS->PlayerArray)
	{
		auto PC = Cast<ARacePlayerController>(Player->GetPlayerController());
		check(PC);
		PC->PlaySequence("Intro");
	}
}

void ARaceGameMode::PostLogin(APlayerController* _NewPlayer)
{
	Super::PostLogin(_NewPlayer);

	ARacePlayerState* RacePlayerState = _NewPlayer->GetPlayerState<ARacePlayerState>();
	check(RacePlayerState);
	RacePlayerState->SetCheckPointIndex(0);
}

bool ARaceGameMode::ReadyToStartMatch_Implementation()
{
	//TODO : 모든 유저의 접속을 확인하는 로직으로 변경
	return 2 <= NumPlayers;
}



void ARaceGameMode::HandlePlayerDeath(APlayerController* _PC) const
{
	ARacePlayerController* PC = Cast<ARacePlayerController>(_PC);
	check(PC);

	PC->Client_ShowCharacterDiedWidget();
}

void ARaceGameMode::DrawNewCharacter(APlayerController* _PC)
{
	ARacePlayerState* PS = _PC->GetPlayerState<ARacePlayerState>();
	check(PS);

	ARacePlayerController* PC = Cast<ARacePlayerController>(_PC);
	check(PC);

	if (CharacterQueue_.IsEmpty())
	{
		ShuffleCharacterQueue();
	}

	UCharacterData* NextCharacterData = *CharacterQueue_.Peek();
	CharacterQueue_.Pop();

	check(NextCharacterData);

	PS->SetCharacterData(NextCharacterData);
	PC->Client_ShowCharacterDrawResult(NextCharacterData);
}

void ARaceGameMode::SpawnNewCharacter(APlayerController* _PC)
{
	ARacePlayerState* PS = _PC->GetPlayerState<ARacePlayerState>();
	check(PS);

	UCharacterData* CharacterData = PS->GetCharacterData();
	check(CharacterData);

	TSubclassOf<ACharacter> NextCharacterClass = CharacterData->GetCharacterClass();

	FTransform SpawnTransform;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = _PC;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	//레이스가 아직 시작하지 않은 첫 Spawn
	//PlayerStart에 Spawn, 레이스 준비가 된 플레이어로 간주함.
	if (false == bRaceStarted_)
	{
		const AActor* PlayerStart = FindPlayerStart(_PC);
		if (PlayerStart)
		{
			SpawnTransform = PlayerStart->GetTransform();
		}
	}
	else
	{
		const uint8 CheckPointIndex = PS->GetCheckPointIndex();
		SpawnTransform = TrackSpline_->GetSplinePointTransform(CheckPointIndex);

		ARacePlayerController* PC = Cast<ARacePlayerController>(_PC);
		if(PC)
		{
			PC->Client_EnableCharacterInput();
		}
	}

	ACharacter* NextCharacter = GetWorld()->SpawnActor<ACharacter>(NextCharacterClass, SpawnTransform, SpawnParams);
	check(NextCharacter);
	if(_PC->GetPawn())
	{
		_PC->GetPawn()->Destroy();
	}
	_PC->Possess(NextCharacter);
}


void ARaceGameMode::HandleCheckPointPassed(uint8 _CheckPointIndex, APlayerController* _PC)
{
	ARacePlayerState* PS = _PC->GetPlayerState<ARacePlayerState>();
	ARaceGameState* GS = GetGameState<ARaceGameState>();
	check(PS && GS);

	const uint8 TargetCheckPointIndex = (PS->GetCheckPointIndex() + 1) % GS->GetMaxCheckPointCount();
	if(_CheckPointIndex != TargetCheckPointIndex)
	{
		return;
	}


	//한바퀴를 돌아 Laps 증가
	if(_CheckPointIndex == 0)
	{
		if (GetGameState<ARaceGameState>()->GetMaxLap() <= PS->GetLaps())
		{
			//완주 처리 코드
			return;
		}
		PS->IncreaseLap();
	}

	APawn* PassedPawn = _PC->GetPawn();
	if (PassedPawn)
	{
		PassedPawn->Destroy();
	}

	PS->SetCheckPointIndex(TargetCheckPointIndex);
	DrawNewCharacter(_PC);
}

void ARaceGameMode::StartRaceCountDown()
{
	ARaceGameState* GS = GetGameState<ARaceGameState>();
	check(GS);

	for(auto PS : GS->PlayerArray)
	{
		auto PC = Cast<ARacePlayerController>(PS->GetPlayerController());
		check(PC);

		PC->Client_StartRaceCountdown();
	}
	bRaceStarted_ = true;
}

TObjectPtr<UCharacterData> ARaceGameMode::PopNextCharacter()
{
	//다음 캐릭터 클래스 선택
	const TObjectPtr<UCharacterData> CharacterData = *CharacterQueue_.Peek();
	CharacterQueue_.Pop();

	return CharacterData;
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

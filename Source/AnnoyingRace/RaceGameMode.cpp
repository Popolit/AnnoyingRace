#include "RaceGameMode.h"

#include "RaceGameInstance.h"
#include "RaceGameState.h"
#include "RacePlayerState.h"
#include "RacePlayerController.h"
#include "Camera/CameraComponent.h"
#include "Characters/PlayableCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "World/TrackSplineActor.h"
#include "Characters/CharacterData.h"
#include "Engine/AssetManager.h"
#include "World/CheckPointBox.h"

ARaceGameMode::ARaceGameMode()
{
	GameStateClass = ARaceGameState::StaticClass();
	PlayerStateClass = ARacePlayerState::StaticClass();
	DefaultPawnClass = nullptr;
	bRaceStarted_ = false;
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

	auto GI = GetGameInstance<URaceGameInstance>();
	check(GI);

	GS->SetMaxLap(GI->GetRaceLaps());
}

void ARaceGameMode::StartMatch()
{
	Super::StartMatch();

	ARaceGameState* GS = GetGameState<ARaceGameState>();
	check(GS);

	TArray<AActor*> CheckPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckPointBox::StaticClass(), CheckPoints);

	ACheckPointBox* StartCheckPoint = nullptr;
	for (AActor* CheckPoint : CheckPoints)
	{
		auto CP = Cast<ACheckPointBox>(CheckPoint);
		if (CP && CP->IsStartPoint())
		{
			StartCheckPoint = CP;
			break;
		}
	}
	for(auto Player : GS->PlayerArray)
	{
		auto PC = Cast<ARacePlayerController>(Player->GetPlayerController());

		if(PC)
		{
			UE_LOG(LogTemp, Display, TEXT("%s - StartMatch"), *PC->PlayerState->GetPlayerName());
			PC->Client_CloseWaitingPlayersUI();
			//인트로 연출 재생
			PC->Client_PlaySequence("Intro");
		}

		auto PS = Cast<ARacePlayerState>(Player);
		if (PS)
		{
			PS->SetCheckPoint(StartCheckPoint);
		}
	}
}

void ARaceGameMode::PostLogin(APlayerController* _NewPlayer)
{
	Super::PostLogin(_NewPlayer);
	auto PC = Cast<ARacePlayerController>( _NewPlayer );
	check(PC);

	PC->Client_OpenWaitingPlayersUI();
}

bool ARaceGameMode::ReadyToStartMatch_Implementation()
{
	Super::ReadyToStartMatch_Implementation();

	URaceGameInstance* GI = GetGameInstance<URaceGameInstance>();
	if (GI)
	{
		const int32 ExpectedPlayerCount = GI->GetRacePlayerCount();
		return 0 < ExpectedPlayerCount && ExpectedPlayerCount == ReadiedPlayerCount_;
	}
	return false;
}

//TODO : Decrease Count가 필요한지 생각해보기
void ARaceGameMode::AddReadiedPlayerCount()
{
	ReadiedPlayerCount_++;
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

	//false인 경우, PlayerStart에서 시작
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
		SpawnTransform = PS->GetSpawnTransform();
		SpawnTransform.SetScale3D(FVector(1,1,1));
	}
	
	auto PC = Cast<ARacePlayerController>(_PC);
	if(PC)
	{
		PC->Client_EnableCharacterInput();
		ACharacter* NextCharacter = GetWorld()->SpawnActor<ACharacter>(NextCharacterClass, SpawnTransform, SpawnParams);
		check(NextCharacter);
		if(PC->GetPawn())
		{
			PC->GetPawn()->Destroy();
		}
		PC->Possess(NextCharacter);
	}
}


void ARaceGameMode::HandleCheckPointPassed(APlayerController* _PC, const ACheckPointBox* _PassedCheckPoint)
{
	ARacePlayerState* PS = _PC->GetPlayerState<ARacePlayerState>();
	ARaceGameState* GS = GetGameState<ARaceGameState>();
	check(PS && GS);

	if (nullptr == _PassedCheckPoint)
	{
		UE_LOG(LogTemp, Error, TEXT("CheckPoint was nullptr"));
		return;
	}

	const ACheckPointBox* PlayerCheckPoint = Cast<ACheckPointBox>(PS->GetPassedCheckPoint());
	if (nullptr == PlayerCheckPoint)
	{
		UE_LOG(LogTemp, Error, TEXT("Player's CheckPoint was nullptr"));
		return;
	}
	
	//목표 체크포인트가 아니면 return
	if (_PassedCheckPoint != PlayerCheckPoint->GetTargetCheckPoint())
	{
		return;
	}

	//시작점 통과 : Laps 증가
	if(_PassedCheckPoint->IsStartPoint())
	{
		//완주 했을 때
		if (GetGameState<ARaceGameState>()->GetMaxLap() <= PS->GetLaps())
		{
			PS->SetRaceFinished();
			auto PC = Cast<ARacePlayerController>(_PC);
			APawn* PassedPawn = _PC->GetPawn();
			FTransform Transform = FTransform::Identity;
			if (PassedPawn)
			{
				auto Camera = PassedPawn->GetComponentByClass<UCameraComponent>();
				if (Camera)
				{
					Transform = Camera->GetComponentTransform();
				}
				PassedPawn->Destroy();
			}
			if (PC)
			{
				PC->SetSpectatorMode(Transform);
				PC->Client_OpenPlayerFinishUI();
			}
			
			//전원 완주
			if (CheckAllPlayersFinishedRace())
			{
				FinishRace();
			}
			return;
		}
		
		PS->IncreaseLap();
	}
	
	APawn* PassedPawn = _PC->GetPawn();
	if (PassedPawn)
	{
		PassedPawn->Destroy();
	}
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

bool ARaceGameMode::CheckAllPlayersFinishedRace() const
{
	auto GS = GetGameState<ARaceGameState>();

	if (GS)
	{
		for (const auto& Elem : GS->PlayerArray)
		{
			auto PS = Cast<ARacePlayerState>(Elem);

			check(PS);
			if (false == PS->IsFinished())
			{
				return false;
			}
		}
	}
	return true;
}

void ARaceGameMode::FinishRace()
{
	auto GS = GetGameState<ARaceGameState>();

	if (GS)
	{
		const TArray<TObjectPtr<APlayerState>> Rankings = GS->GetPlayerRankings();
		int32 PlayerRank = 1;
		for (const auto PS : Rankings)
		{
			auto PC = Cast<ARacePlayerController>(PS->GetOwningController());

			if (PC)
			{
				PC->Client_OpenRaceFinishedUI(PlayerRank);
			}
			PlayerRank++;
		}

		GetWorldTimerManager().SetTimer(RaceFinishCountDownTimer_, this,
			&ARaceGameMode::ReturnToSession, 5.0f, false);
	}
}

void ARaceGameMode::ReturnToSession()
{
	auto GI = GetGameInstance<URaceGameInstance>();
	auto GS = GetGameState<ARaceGameState>();
	if (GI && GS)
	{
		GI->SetPrevGameResult(GS->GetPlayerRankings());
		FPrimaryAssetId SessionMap = GI->GetSessionMap();
		if (SessionMap.IsValid())
		{
			FAssetData MapAssetData;
			if (UAssetManager::Get().GetPrimaryAssetData(SessionMap, MapAssetData))
			{
				ProcessServerTravel(MapAssetData.PackageName.ToString() + "?listen", true);
			}
		}
	}
}

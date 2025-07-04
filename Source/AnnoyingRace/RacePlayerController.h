#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RacePlayerController.generated.h"

/**
 * Race PlayerController
 */
UCLASS()
class ANNOYINGRACE_API ARacePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

public:
	//인트로 시퀀스 종료 후 멀티캐스트
	UFUNCTION(NetMulticast, Reliable)
		void Multicast_OnIntroSequenceFinished();

	UFUNCTION(Server, Reliable)
		void Server_RequestDrawCharacter();

	//인스로 시퀀스 종료 바인딩 함수(서버)
	UFUNCTION()
		void OnIntroSequenceFinished();

	UFUNCTION(Client, Reliable)
		void StartRaceCountdown(float _CountDownStartTime);

	UFUNCTION(Client, Reliable)
		void DrawCharacter();

private:
	void Server_RequestDrawCharacter_Implementation();

private:
	UPROPERTY()
		TObjectPtr<UUserWidget> CountdownWidget_;

	UPROPERTY()
		TObjectPtr<UUserWidget> DrawCharacterWidget_;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUserWidget> CountdownWidgetClass_;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUserWidget> DrawCharacterWidgetClass_;
};

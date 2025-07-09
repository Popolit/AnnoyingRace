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

	UFUNCTION()
		void OnIntroSequenceFinished();

	UFUNCTION(Client, Reliable)
		void Client_StartRaceCountdown();

	UFUNCTION(Client, Reliable)
		void Client_ShowCharacterDrawResult(const class UCharacterData* _DrawnCharacterData);

private:
	UFUNCTION(Server, Reliable)
		void Server_RequestDrawCharacter();

	UFUNCTION(Server, Reliable)
		void Server_RequestSpawnCharacter();

	void OnDrawAnimationFinished();

	void OnCountDownAnimationFinished();

private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCountDownWidget> CountDownWidgetClass_;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UDrawCharacterWidget> DrawCharacterWidgetClass_;

	UPROPERTY()
		TObjectPtr<UCountDownWidget> CountdownWidget_;

	UPROPERTY()
		TObjectPtr<UDrawCharacterWidget> DrawCharacterWidget_;
};

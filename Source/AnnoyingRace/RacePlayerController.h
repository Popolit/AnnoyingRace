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
	void PlaySequence(const FName& _SequenceName);

	UFUNCTION(Client, Reliable)
		void Client_EnableCharacterInput();

	UFUNCTION(Client, Reliable)
		void Client_DisableCharacterInput();

	UFUNCTION(Client, Reliable)
		void Client_ShowCharacterDiedWidget();

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

	void OnCharacterDiedAnimationFinished();

	void UpdateDistanceAlongSpline();

private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCountDownWidget> CountDownWidgetClass_;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UDrawCharacterWidget> DrawCharacterWidgetClass_;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCharacterDiedWidget> CharacterDiedWidgetClass_;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUserWidget> PlayerHUDWidgetClass_;

	UPROPERTY()
		TObjectPtr<UCountDownWidget> CountdownWidget_;

	UPROPERTY()
		TObjectPtr<UDrawCharacterWidget> DrawCharacterWidget_;

	UPROPERTY()
		TObjectPtr<UCharacterDiedWidget> CharacterDiedWidget_;

	UPROPERTY()
		TObjectPtr<UUserWidget> PlayerHUDWidget_;

private:
	UPROPERTY()
		TObjectPtr<class ATrackSplineActor> TrackSplineActor_;

	//캐릭터 인풋
	UPROPERTY(EditDefaultsOnly, Category = Input)
		TObjectPtr<class UInputMappingContext> IMC_Character_;

	//공용 인풋 (Ex : 메뉴 열기 등)
	UPROPERTY(EditDefaultsOnly, Category = Input)
		TObjectPtr<class UInputMappingContext> IMC_Default_;

	FTimerHandle DistanceUpdateTimerHandle_;

	bool bEnableCharacterInput_ = false;
};
